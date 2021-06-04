#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "qt_common/s4qt_itemFormatDelegate.h"
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>

namespace S4
{

    class snapInfo : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<int, QVariant> mapTimeout;

        std::vector<QString> _row_names = {
            "Price",
            "Last Close",
            "Open",
            "High",
            "Low",
            "Total Volume",
            "Total Amount",
            "Current Volume",
            "Sell Volume",
            "Buy Volume",
            "Active",
            "Local Time",
            };
        std::vector<QVariant> _data;
        QTimeLine* _timeLine;
    public:
        snapInfo(QObject *parent = {}) : QAbstractTableModel{parent}
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
                return _row_names[index.row()];
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
            case 0: return "Key";
            case 1: return "Value";
            //case 2: return "Reg.#";
            default: return {};
            }

        }

        void refresh(const tdx_snap_t &snap)
        {

            std::vector<QVariant> data;
            for (auto& key : _row_names){
                if (key == "Price") data.push_back(snap.price);
                if (key == "Last Close") data.push_back(snap.last_close);
                if (key == "Open") data.push_back(snap.open);
                if (key == "High") data.push_back(snap.high);
                if (key == "Low") data.push_back(snap.low);
                if (key == "Total Volume") data.push_back(snap.vol);
                if (key == "Total Amount") data.push_back(QString::fromStdString(DoubleConvertor::convert((snap.amount)/_KW) + " KW"));
                if (key == "Current Volume") data.push_back(snap.cur_vol);
                if (key == "Sell Volume") data.push_back(snap.s_vol);
                if (key == "Buy Volume") data.push_back(snap.b_vol);
                if (key == "Active") data.push_back(snap.active1);
                if (key == "Local Time") data.push_back(snap.minuSec);
            }

            //backup
            beginResetModel();
            std::swap(data, _data);
            endResetModel();

            //高亮变动
			_timeLine->stop();
			mapTimeout.clear();
			for (int i = 0; i < _row_names.size(); ++i) {
                if (data[i] != _data[i]) {
					mapTimeout.insert((i) * 100 + 1, QDateTime::currentDateTime());
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
    };

}