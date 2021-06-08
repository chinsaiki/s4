#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "qt_common/s4qt_itemFormatDelegate.h"
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>

#include "sbe_ssz.h"
#include "sbe_ssh.h"

namespace S4
{

    class snapTableModel_level : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<int, QVariant> mapTimeout;

        std::vector<QString> _title = {"Side", "Price", "Volume"};
        const int _side_levels_nb;
        
        struct Level_t
        {
            fprice_t price = 0;     //
            vol_board_t vol = 0;	//手
            Level_t(fprice_t p = 0, vol_board_t v = 0):
                price(p), vol(v)
            {}
        };

        std::vector<Level_t> _ask; //0 is best
        std::vector<Level_t> _bid; //0 is best
        QTimeLine* _timeLine;
    public:
        snapTableModel_level(int side_levels_nb, QObject *parent = {}) : QAbstractTableModel{parent},
                                                               _side_levels_nb(side_levels_nb)
        {
            _ask.resize(side_levels_nb);
			_bid.resize(side_levels_nb);
			_timeLine = new QTimeLine(itemFormatDelegate::update_scope + 200, this);
            _timeLine->setFrameRange(0, itemFormatDelegate::update_nb);
            _timeLine->stop();
			connect(_timeLine, &QTimeLine::frameChanged, this, [=](int ) {
				beginResetModel();
				endResetModel();
			});
		}

        int rowCount(const QModelIndex &) const override { return _side_levels_nb << 1; }
        int columnCount(const QModelIndex &) const override { return (int)_title.size(); }
        QVariant data(const QModelIndex &index, int role) const override
        {
            if (role == itemFormatDelegateRole) {
                return itemFadeColor(index);
            }

			if (role != Qt::DisplayRole)
				return {}; // && role != Qt::EditRole
            int side_index = index.row() < _side_levels_nb ? _side_levels_nb - index.row() - 1 : index.row() - _side_levels_nb;
            const auto &levle = index.row() < _side_levels_nb ? _ask[side_index] : _bid[side_index];
            const QString side = index.row() < _side_levels_nb ? "BID":"ASK";
            switch (index.column())
            {
            case 0:
                return side + QString::number(side_index+1);
            case 1:
                return levle.price;
            case 2:
                return levle.vol;
            default:
                return {};
            };
        }
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override
        {
            if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
                return {};
            if ((size_t)section >= _title.size())
                return {};
            //switch (section) {
            //case 0: return "Make";
            //case 1: return "Model";
            //case 2: return "Reg.#";
            //default: return {};
            return _title[section];
        }

        void refresh(const tdx_snap_t &snap)
        {
            std::vector<Level_t> ask;
            std::vector<Level_t> bid;
            ask.push_back({ iPrice_to_fPrice(snap.ask1), snap.ask_vol1 });
            ask.push_back({ iPrice_to_fPrice(snap.ask2), snap.ask_vol2 });
            ask.push_back({ iPrice_to_fPrice(snap.ask3), snap.ask_vol3 });
            ask.push_back({ iPrice_to_fPrice(snap.ask4), snap.ask_vol4 });
            ask.push_back({ iPrice_to_fPrice(snap.ask5), snap.ask_vol5 });
            bid.push_back({ iPrice_to_fPrice(snap.bid1), snap.bid_vol1 });
            bid.push_back({ iPrice_to_fPrice(snap.bid2), snap.bid_vol2 });
            bid.push_back({ iPrice_to_fPrice(snap.bid3), snap.bid_vol3 });
            bid.push_back({ iPrice_to_fPrice(snap.bid4), snap.bid_vol4 });
            bid.push_back({ iPrice_to_fPrice(snap.bid5), snap.bid_vol5 });

            refresh(ask, bid);
        }

        void refreshL2(const std::string& l2data){
            size_t sbe_size = l2data.size();
            if (sbe_size < sizeof(SBE_SSH_header_t)){
                return;
            }
            const SBE_SSH_header_t* pH = (SBE_SSH_header_t*)l2data.data();
            std::vector<Level_t> ask;
            std::vector<Level_t> bid;

            if (pH->MsgType == __MsgType_SSH_INSTRUMENT_SNAP__ && pH->MsgLen == sizeof(SBE_SSH_instrument_snap_t)){
                const SBE_SSH_instrument_snap_t* pSnap = (SBE_SSH_instrument_snap_t*)l2data.data();
                for (int i=0; i<10 && i<_side_levels_nb; ++i){
                    ask.push_back(Level_t( L2_iPrice_snap_to_fPrice(pSnap->AskLevel[i].Price), L2_Qty_to_hand(pSnap->AskLevel[i].Qty) ));
                    bid.push_back(Level_t( L2_iPrice_snap_to_fPrice(pSnap->BidLevel[i].Price), L2_Qty_to_hand(pSnap->BidLevel[i].Qty) ));
                }
            }else if (pH->MsgType == __MsgType_SSZ_INSTRUMENT_SNAP__ && pH->MsgLen == sizeof(SBE_SSZ_instrument_snap_t)){
                const SBE_SSZ_instrument_snap_t* pSnap = (SBE_SSZ_instrument_snap_t*)l2data.data();
                for (int i=0; i<10 && i<_side_levels_nb; ++i){
                    ask.push_back(Level_t( L2_iPrice_snap_to_fPrice(pSnap->AskLevel[i].Price), L2_Qty_to_hand(pSnap->AskLevel[i].Qty) ));
                    bid.push_back(Level_t( L2_iPrice_snap_to_fPrice(pSnap->BidLevel[i].Price), L2_Qty_to_hand(pSnap->BidLevel[i].Qty) ));
                }
            }

            refresh(ask, bid);
        }

    private:
        QVariant itemFadeColor(const QModelIndex& index) const
        {
            QMap<int, QVariant>::const_iterator it = mapTimeout.find(index.row() * 100 + index.column());
            if (it == mapTimeout.end()) return QVariant();
            float nTimePassed = it.value().toDateTime().msecsTo(QDateTime::currentDateTime());
            if (nTimePassed < itemFormatDelegate::update_scope) {
                float idx = nTimePassed / itemFormatDelegate::update_scope;
                QColor bg = Qt::cyan;
                uint8_t r = (255 - bg.red()) * (idx)+bg.red();
                uint8_t g = (255 - bg.green()) * (idx)+bg.green();
                uint8_t b = (255 - bg.blue()) * (idx)+bg.blue();
                //bg.setAlpha(0.2);
                return QColor(r, g, b);
            }
            return  QColor(255, 255, 255);
        }

        void refresh(std::vector<Level_t>& ask, std::vector<Level_t>& bid)
        {
            //backup
            beginResetModel();
            std::swap(ask, _ask);
            std::swap(bid, _bid);
            endResetModel();

            //高亮变动
            _timeLine->stop();
            mapTimeout.clear();
            for (int i = 0; i < _side_levels_nb; ++i) {
                if (ask[i].vol != _ask[i].vol) {
                    mapTimeout.insert((_side_levels_nb - 1 - i) * 100 + 2, QDateTime::currentDateTime());
                }
                if (bid[i].vol != _bid[i].vol) {
                    mapTimeout.insert((_side_levels_nb + i) * 100 + 2, QDateTime::currentDateTime());
                }
                if (ask[i].price != _ask[i].price) {
                    mapTimeout.insert((_side_levels_nb - 1 - i) * 100 + 1, QDateTime::currentDateTime());
                }
                if (bid[i].price != _bid[i].price) {
                    mapTimeout.insert((_side_levels_nb + i) * 100 + 1, QDateTime::currentDateTime());
                }
            }
            if (mapTimeout.size())
                _timeLine->start();
        }
    };

}