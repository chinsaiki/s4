#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "data/s4infSnap.h"
#include "qt_common/s4qt_itemFormatDelegate.h"
#include "qt_common/sharedCharArray_ptr.h"
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>

#include "sbe_ssz.h"
#include "sbe_ssh.h"

namespace S4
{

    class snapTableModel_snapInfo_L2 : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<size_t, QVariant> mapTimeout;

        enum class dataType_t{
            Price,
            LastClose,
            Open,
            High,
            Low,
            TotalVolume,
            TotalAmount,
            NumTrades,
            AskWeightPx,
            AskWeightSize,
            BidWeightPx,
            BidWeightSize,
            TransactTime,
        };

        std::vector<dataType_t> _row_names = {
            dataType_t::Price,
            dataType_t::LastClose,
            dataType_t::Open,
            dataType_t::High,
            dataType_t::Low,
            dataType_t::TotalVolume,
            dataType_t::TotalAmount,
            dataType_t::NumTrades,
            dataType_t::AskWeightPx,
            dataType_t::AskWeightSize,
            dataType_t::BidWeightPx,
            dataType_t::BidWeightSize,
            dataType_t::TransactTime,
            };
        std::vector<QVariant> _data;
        QTimeLine* _timeLine;
    public:
        snapTableModel_snapInfo_L2(QObject *parent = {}) : QAbstractTableModel{parent}
        {
            _data.resize(_row_names.size());

			_timeLine = new QTimeLine(itemFormatDelegate::update_scope + 200, this);
            _timeLine->setFrameRange(0, itemFormatDelegate::update_nb);
            _timeLine->stop();
			connect(_timeLine, &QTimeLine::frameChanged, this, [=](int ) {
				beginResetModel();
				endResetModel();
			});
		}

        int rowCount(const QModelIndex &) const override { return (int)_row_names.size(); }
        int columnCount(const QModelIndex &) const override { return 2; }
        QVariant data(const QModelIndex &index, int role) const override
        {
            if (role == itemFormatDelegateRole) {
                return itemFadeColor(index);
            }

			if (role != Qt::DisplayRole)
				return {}; // && role != Qt::EditRole
            switch (index.column())
            {
            case 0:
                return typeString(_row_names[index.row()]);
            case 1:
                return _data[index.row()];
            default:
                return {};
            };
        }
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override
        {
            if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
                return {};
            if ((size_t)section >= 2)
                return {};
            switch (section) {
            case 0: return "";
            case 1: return "";
            //case 2: return "Reg.#";
            default: return {};
            }

        }
        
        void refreshL2(const S4::sharedCharArray_ptr& l2data){
            size_t sbe_size = l2data->size();
            if (sbe_size < sizeof(SBE_SSH_header_t)){
                return;
            }
            const SBE_SSH_header_t* pH = (SBE_SSH_header_t*)l2data->get();
            std::vector<QVariant> data;

            if (pH->SecurityIDSource == 101 && pH->MsgType == __MsgType_SSH_INSTRUMENT_SNAP__ && pH->MsgLen == sizeof(SBE_SSH_instrument_snap_t)){
                const SBE_SSH_instrument_snap_t* pSnap = (SBE_SSH_instrument_snap_t*)l2data->get();
                for (auto& key : _row_names){
                    if (key == dataType_t::Price) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->LastPx)).c_str());
                    if (key == dataType_t::LastClose) data.push_back(priceString(L2_iPrice_tick_to_fPrice(pSnap->PrevClosePx)).c_str());
                    if (key == dataType_t::Open) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->OpenPx)).c_str());
                    if (key == dataType_t::High) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->HighPx)).c_str());
                    if (key == dataType_t::Low) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->LowPx)).c_str());
                    if (key == dataType_t::TotalVolume) data.push_back(QVariant::fromValue(pSnap->TotalVolumeTrade/L2_Qty_precision));
                    if (key == dataType_t::TotalAmount) data.push_back(QString::number((pSnap->TotalValueTrade/L2_Amt_precision)/_KW) + QStringLiteral(" 千万"));
                    if (key == dataType_t::NumTrades) data.push_back(pSnap->NumTrades);
                    if (key == dataType_t::AskWeightPx) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->AskWeightPx)).c_str());
                    if (key == dataType_t::AskWeightSize) data.push_back(QVariant::fromValue(pSnap->AskWeightSize/L2_Qty_precision));
                    if (key == dataType_t::BidWeightPx) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->BidWeightPx)).c_str());
                    if (key == dataType_t::BidWeightSize) data.push_back(QVariant::fromValue(pSnap->BidWeightSize/L2_Qty_precision));
                    if (key == dataType_t::TransactTime) data.push_back(pSnap->DataTimeStamp);
                }
            }else 
            if (pH->SecurityIDSource == 102 && pH->MsgType == __MsgType_SSZ_INSTRUMENT_SNAP__ && pH->MsgLen == sizeof(SBE_SSZ_instrument_snap_t)){
                const SBE_SSZ_instrument_snap_t* pSnap = (SBE_SSZ_instrument_snap_t*)l2data->get();
                for (auto& key : _row_names){
                    if (key == dataType_t::Price) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->LastPx)).c_str());
                    if (key == dataType_t::LastClose) data.push_back(priceString(L2_iPrice_tick_to_fPrice(pSnap->PrevClosePx)).c_str());
                    if (key == dataType_t::Open) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->OpenPx)).c_str());
                    if (key == dataType_t::High) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->HighPx)).c_str());
                    if (key == dataType_t::Low) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->LowPx)).c_str());
                    if (key == dataType_t::TotalVolume) data.push_back(QVariant::fromValue(pSnap->TotalVolumeTrade/L2_Qty_precision));
                    if (key == dataType_t::TotalAmount) data.push_back(QString::number((pSnap->TotalValueTrade/L2_Amt_precision)/_KW) + QStringLiteral(" 千万"));
                    if (key == dataType_t::NumTrades) data.push_back(QString::number(pSnap->NumTrades));
                    if (key == dataType_t::AskWeightPx) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->AskWeightPx)).c_str());
                    if (key == dataType_t::AskWeightSize) data.push_back(QVariant::fromValue(pSnap->AskWeightSize/L2_Qty_precision));
                    if (key == dataType_t::BidWeightPx) data.push_back(priceString(L2_iPrice_snap_to_fPrice(pSnap->BidWeightPx)).c_str());
                    if (key == dataType_t::BidWeightSize) data.push_back(QVariant::fromValue(pSnap->BidWeightSize/L2_Qty_precision));
                    if (key == dataType_t::TransactTime) data.push_back(ssz_L2_timeString(pSnap->TransactTime).c_str());
		        }
            }
            
            refresh(data);
        }


    private:
        QVariant itemFadeColor(const QModelIndex& index) const
        {
            QMap<size_t, QVariant>::const_iterator it = mapTimeout.find(index.row() * 100 + index.column());
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

        void refresh(std::vector<QVariant>& data)
        {
            //backup
            beginResetModel();
            std::swap(data, _data);
            endResetModel();

            //高亮变动
            _timeLine->stop();
            mapTimeout.clear();
            for (size_t i = 0; i < _row_names.size(); ++i) {
                if (data[i] != _data[i]) {
                    mapTimeout.insert((i) * 100 + 1, QDateTime::currentDateTime());
                }
            }
            if (mapTimeout.size())
                _timeLine->start();
        }

        QString typeString(dataType_t t) const
        {
            switch (t)
            {
            case dataType_t::Price: return QStringLiteral("价格");
            case dataType_t::LastClose: return QStringLiteral("昨收");
            case dataType_t::Open: return QStringLiteral("开盘价");
            case dataType_t::High: return QStringLiteral("最高");
            case dataType_t::Low: return QStringLiteral("最低");
            case dataType_t::TotalVolume: return QStringLiteral("总量");
            case dataType_t::TotalAmount: return QStringLiteral("总额");
            case dataType_t::NumTrades: return QStringLiteral("成交订单数");
            case dataType_t::AskWeightPx: return QStringLiteral("委卖加权价格");
            case dataType_t::AskWeightSize: return QStringLiteral("委卖量");
            case dataType_t::BidWeightPx: return QStringLiteral("委买加权价格");
            case dataType_t::BidWeightSize: return QStringLiteral("委买量");
            case dataType_t::TransactTime: return QStringLiteral("时间戳");
            default:return "";
            }
        }
    };

}
