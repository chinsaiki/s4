#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "qt_common/s4qt_tableDelegate.h"
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>

namespace S4
{

    class snapLevels : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<int, QVariant> mapTimeout;

        std::vector<QString> _title = {"Side", "Price", "Volume"};
        const int _side_levels_nb;
        std::vector<orderBookLevel_t> _ask; //0 is best
        std::vector<orderBookLevel_t> _bid; //0 is best
        QTimeLine* _timeLine;
    public:
        snapLevels(int side_levels_nb, QObject *parent = {}) : QAbstractTableModel{parent},
                                                               _side_levels_nb(side_levels_nb)
        {
            _ask.resize(side_levels_nb);
			_bid.resize(side_levels_nb);
			_timeLine = new QTimeLine(tableDelegate::update_scope + 200, this);
            _timeLine->setFrameRange(0, tableDelegate::update_nb);
            _timeLine->stop();
			connect(_timeLine, &QTimeLine::frameChanged, this, [=](int ) {
				beginResetModel();
				endResetModel();
			});
			//connect(_timeLine, &QTimeLine::finished, this, [=]() {
   //             mapTimeout.clear();
			//	beginResetModel();
			//	endResetModel();
			//});
		}

        int rowCount(const QModelIndex &) const override { return _side_levels_nb << 1; }
        int columnCount(const QModelIndex &) const override { return (int)_title.size(); }
        QVariant data(const QModelIndex &index, int role) const override
        {
            if (role == ItemChangeNoticeRole) {
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
            std::vector<orderBookLevel_t> ask;
            std::vector<orderBookLevel_t> bid;
            ask.push_back({ orderSide_t::ASK, snap.ask1, snap.ask_vol1 });
            ask.push_back({ orderSide_t::ASK, snap.ask2, snap.ask_vol2 });
            ask.push_back({ orderSide_t::ASK, snap.ask3, snap.ask_vol3 });
            ask.push_back({ orderSide_t::ASK, snap.ask4, snap.ask_vol4 });
            ask.push_back({ orderSide_t::ASK, snap.ask5, snap.ask_vol5 });
            bid.push_back({ orderSide_t::BID, snap.bid1, snap.bid_vol1 });
            bid.push_back({ orderSide_t::BID, snap.bid2, snap.bid_vol2 });
            bid.push_back({ orderSide_t::BID, snap.bid3, snap.bid_vol3 });
            bid.push_back({ orderSide_t::BID, snap.bid4, snap.bid_vol4 });
            bid.push_back({ orderSide_t::BID, snap.bid5, snap.bid_vol5 });
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
        private:
            QVariant itemFadeColor(const QModelIndex& index) const
			{
				QMap<int, QVariant>::const_iterator it = mapTimeout.find(index.row() * 100 + index.column());
				if (it == mapTimeout.end()) return QVariant();
				float nTimePassed = it.value().toDateTime().msecsTo(QDateTime::currentDateTime());
				if (nTimePassed < tableDelegate::update_scope) {
					float idx = nTimePassed / tableDelegate::update_scope;
					QColor bg = Qt::cyan;
					uint8_t r = (255 - bg.red()) * (idx)+bg.red();
					uint8_t g = (255 - bg.green()) * (idx)+bg.green();
					uint8_t b = (255 - bg.blue()) * (idx)+bg.blue();
					//bg.setAlpha(0.2);
					return QColor(r, g, b);
				}
				return  QColor(255, 255, 255);
            }
    };

}