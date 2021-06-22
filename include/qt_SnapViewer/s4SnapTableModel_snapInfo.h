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

    class snapTableModel_snapInfo : public QAbstractTableModel
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
            CurrentVolume,
            SellVolume,
            BuyVolume,
            Active,
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
            dataType_t::CurrentVolume,
            dataType_t::SellVolume,
            dataType_t::BuyVolume,
            dataType_t::Active,
            dataType_t::TransactTime,
            };
        std::vector<QVariant> _data;
        QTimeLine* _timeLine;

    public:
        snapTableModel_snapInfo(QObject *parent = {}) : QAbstractTableModel{parent}
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

        void refresh(const infSnap_t* snap)
        {
            std::vector<QVariant> data;
            for (auto& key : _row_names){
                if (key == dataType_t::Price) data.push_back(priceString(iPrice_to_fPrice(snap->price)).c_str());
                if (key == dataType_t::LastClose) data.push_back(priceString(iPrice_to_fPrice(snap->last_close)).c_str());
                if (key == dataType_t::Open) data.push_back(priceString(iPrice_to_fPrice(snap->open)).c_str());
                if (key == dataType_t::High) data.push_back(priceString(iPrice_to_fPrice(snap->high)).c_str());
                if (key == dataType_t::Low) data.push_back(priceString(iPrice_to_fPrice(snap->low)).c_str());
                if (key == dataType_t::TotalVolume) data.push_back(snap->vol);
                if (key == dataType_t::TotalAmount) data.push_back(QString::number((snap->amount)/_KW) + QStringLiteral(" 千万"));
                if (key == dataType_t::CurrentVolume) data.push_back(snap->cur_vol);
                if (key == dataType_t::SellVolume) data.push_back(snap->s_vol); //主动卖出
                if (key == dataType_t::BuyVolume) data.push_back(snap->b_vol);  //主动买入
                if (key == dataType_t::Active) data.push_back(snap->active1);
                if (key == dataType_t::TransactTime) data.push_back(snap->_MinmuSec);
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

        static
        QString typeString(dataType_t t)
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
            case dataType_t::CurrentVolume: return QStringLiteral("现量");
            case dataType_t::SellVolume: return QStringLiteral("主动卖出");
            case dataType_t::BuyVolume: return QStringLiteral("主动买入");
            case dataType_t::Active: return QStringLiteral("Active");
            case dataType_t::TransactTime: return QStringLiteral("时间戳");
            default:return "";
            }
        }

        


    };

}
