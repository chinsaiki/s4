#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
#include "qt_common/s4qt_itemFormatDelegate.h"
#include "qt_common/sharedCharArray_ptr.h"
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>

#include "sbe_ssz.h"
#include "sbe_ssh.h"

namespace S4
{

    class snapTableModel_order : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<size_t, QVariant> mapTimeout;

        enum class dataType_t{
            OrderNo,
            Price,
            OrderQty,
            Side,
            OrdType,
            OrderTime,
        };

        std::vector<dataType_t> _title = {
            dataType_t::OrderNo,
            dataType_t::Price,
            dataType_t::OrderQty,
            dataType_t::Side,
            dataType_t::OrdType,
            dataType_t::OrderTime,
        };

        struct unionOrderType_t{
            int64_t         OrderNo;    //
            int32_t         Price;      //
            int64_t         OrderQty;
            int8_t          Side;       //'B', 'S'
            int8_t          OrdType;    //'A', 'D'
            uint32_t        OrderTime;
        };

        QList<unionOrderType_t> _data;

        QTimeLine* _timeLine;
    public:
        snapTableModel_order(QObject *parent = {}) : QAbstractTableModel{parent}
        {
			_timeLine = new QTimeLine(itemFormatDelegate::update_scope + 200, this);
            _timeLine->setFrameRange(0, itemFormatDelegate::update_nb);
            _timeLine->stop();
			connect(_timeLine, &QTimeLine::frameChanged, this, [=](int ) {
				beginResetModel();
				endResetModel();
			});
		}

        int rowCount(const QModelIndex &) const override { return (int)_data.count(); }
        int columnCount(const QModelIndex &) const override { return (int)_title.size(); }
        QVariant data(const QModelIndex &index, int role) const override
        {
            if (role == itemFormatDelegateRole) {
                return itemFadeColor(index);
            }

			if (role != Qt::DisplayRole)
				return {}; // && role != Qt::EditRole
            const auto& order = _data[index.row()];
            switch (index.column()) {
            case 0: return QVariant::fromValue(order.OrderNo);
            case 1: return order.Price;
            case 2: return order.OrderQty;
            case 3: return order.Side;
            case 4: return order.OrdType;
            case 5: return QVariant::fromValue(order.OrderTime);
            default: return {};
            };
        }
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override
        {
            if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
                return {};
            if ((size_t)section >= _title.size()) return {};
            return typeString(_title[section]);
        }
        
        void refreshL2(const sharedCharArray_ptr& l2data){
            // qDebug() << "ref on order: "<< l2data->curRef();
            size_t sbe_size = l2data->size();
            if (sbe_size < sizeof(SBE_SSH_header_t)){
                return;
            }
            const SBE_SSH_header_t* pH = (SBE_SSH_header_t*)l2data->get();
            unionOrderType_t data;

            if (pH->SecurityIDSource == 101 && pH->MsgType == __MsgType_SSH_ORDER__ && pH->MsgLen == sizeof(SBE_SSH_ord_t)){
                const SBE_SSH_ord_t* pOrder = (SBE_SSH_ord_t*)l2data->get();
                data.OrderNo = pOrder->OrderNo;
                data.Price = pOrder->Price;
                data.OrderQty = pOrder->OrderQty;
                data.Side = pOrder->Side;
                data.OrdType = pOrder->OrdType;
                data.OrderTime = pOrder->OrderTime;
            }else 
            if (pH->SecurityIDSource == 102 && pH->MsgType == __MsgType_SSZ_ORDER__ && pH->MsgLen == sizeof(SBE_SSZ_ord_t)){
                const SBE_SSZ_ord_t* pOrder = (SBE_SSZ_ord_t*)l2data->get();
                data.OrderNo = pH->ApplSeqNum;
                data.Price = pOrder->Price;
                data.OrderQty = pOrder->OrderQty;
                data.Side = pOrder->Side;
                data.OrdType = pOrder->OrdType;
                data.OrderTime = pOrder->TransactTime;
            }
            beginInsertRows({}, 0, 0);
            _data.append(data);
            endInsertRows();

            _timeLine->stop();
            auto now = QDateTime::currentDateTime();
            for (size_t i = mapTimeout.size(); i > 0; --i) {
                if (mapTimeout[i-1].toDateTime().msecsTo(now) <= itemFormatDelegate::update_scope){
                    mapTimeout.insert(i, mapTimeout[i-1]);
                }else{
                    mapTimeout.remove(i-1);
                }
            }
            mapTimeout.insert(0, QDateTime::currentDateTime());
            _timeLine->start();
        }


    private:
        QVariant itemFadeColor(const QModelIndex& index) const
        {
            QMap<size_t, QVariant>::const_iterator it = mapTimeout.find(index.row());
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

        QString typeString(dataType_t t) const
        {
            switch (t)
            {
            case dataType_t::OrderNo: return "OrderNo";
            case dataType_t::Price: return "Price Close";
            case dataType_t::OrderQty: return "OrderQty";
            case dataType_t::Side: return "Side";
            case dataType_t::OrdType: return "OrdType";
            case dataType_t::OrderTime: return "OrderTime";
            default:return "";
            }
        }
    };

}
