#pragma once

#include <QTableView>
#include "common/s4time.h"
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

        struct unionOrder_t{
            int64_t         OrderNo;    //
            float           Price;      //
            int64_t         OrderQty;
            QString         Side;       //'B', 'S'
            QString         OrdType;    //'A', 'D'
            QString         OrderTime;
            bool            isBid;
        };

        QList<unionOrder_t> _data;

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
            case 1: return priceString(order.Price).c_str();
	        case 2: return QVariant::fromValue(order.OrderQty);
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
        
        void refreshL2(const S4::sharedCharArray_ptr& l2data){
            // qDebug() << "ref on order: "<< l2data->curRef();
            size_t sbe_size = l2data->size();
            if (sbe_size < sizeof(SBE_SSH_header_t)){
                return;
            }
            const SBE_SSH_header_t* pH = (SBE_SSH_header_t*)l2data->get();
            unionOrder_t data;

            if (pH->SecurityIDSource == 101 && pH->MsgType == __MsgType_SSH_ORDER__ && pH->MsgLen == sizeof(SBE_SSH_ord_t)){
                const SBE_SSH_ord_t* pOrder = (SBE_SSH_ord_t*)l2data->get();
                data.OrderNo = pOrder->OrderNo;
                data.Price = L2_iPrice_tick_to_fPrice(pOrder->Price);
                data.OrderQty = L2_Qty_to_hand(pOrder->OrderQty);
                data.Side = sshSideString(pOrder->Side);
                data.OrdType = sshOrderTypeString(pOrder->OrdType);
                data.OrderTime = ssh_L2_timeString(pOrder->OrderTime).c_str();
                data.isBid = pOrder->Side == 'B';
            }else 
            if (pH->SecurityIDSource == 102 && pH->MsgType == __MsgType_SSZ_ORDER__ && pH->MsgLen == sizeof(SBE_SSZ_ord_t)){
                const SBE_SSZ_ord_t* pOrder = (SBE_SSZ_ord_t*)l2data->get();
                data.OrderNo = pH->ApplSeqNum;
                data.Price = L2_iPrice_tick_to_fPrice(pOrder->Price);
                data.OrderQty = L2_Qty_to_hand(pOrder->OrderQty);
                data.Side = sszSideString(pOrder->Side);
                data.OrdType = sszOrderTypeString(pOrder->OrdType);
                data.OrderTime = ssz_L2_timeString(pOrder->TransactTime).c_str();
                data.isBid = (pOrder->Side == '1' || pOrder->Side == 'G');
            }
            
            if (_data.size() < 200){
                beginInsertRows({}, 0, 0);
                    _data.push_front(data);
                endInsertRows();
            }else{
                beginResetModel();
                    _data.push_front(data);
                    while(_data.size() > 200){
                        _data.pop_back();
                    }
                endResetModel();
            }

            _timeLine->stop();
            auto now = QDateTime::currentDateTime();
            for (size_t i = mapTimeout.size(); i > 0; --i) {
                if (mapTimeout[i-1].toDateTime().msecsTo(now) <= itemFormatDelegate::update_scope && i - 1 < 200){
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
            if (index.column() != 3){   //side以外
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
            }else{
                const auto& order = _data[index.row()];
                if (order.isBid){
                    return  QColor(255, 128, 128);
                }else{
                    return  QColor(128, 255, 128);
                }
            }
        }

        QString typeString(dataType_t t) const
        {
            switch (t)
            {
            case dataType_t::OrderNo: return QStringLiteral("订单编号");
            case dataType_t::Price: return QStringLiteral("价格");
            case dataType_t::OrderQty: return QStringLiteral("手数");
            case dataType_t::Side: return QStringLiteral("方向");
            case dataType_t::OrdType: return QStringLiteral("类型");
            case dataType_t::OrderTime: return QStringLiteral("时间戳");
            default:return "";
            }
        }

        
        QString sszOrderTypeString(uint8_t OrderType) const
        {
            if (OrderType == '1'){
                return QStringLiteral("市价单");
            }else if (OrderType == '2'){
                return QStringLiteral("限价单");
            }else if (OrderType == 'U'){
                return QStringLiteral("本方最优");
            }else{
                return QStringLiteral("未知类型") + (char)(OrderType);
            }
        }

        QString sshOrderTypeString(uint8_t OrderType) const
        {
            if (OrderType == 'A'){
                return QStringLiteral("新增委托订单");
            }else if (OrderType == 'D'){
                return QStringLiteral("删除委托订单");
            }else{
                return QStringLiteral("未知类型") + (char)(OrderType);
            }
        }
        
        QString sszSideString(uint8_t Side) const
        {
            if (Side == '1'){
                return QStringLiteral("买入");
            }else if (Side == '2'){
                return QStringLiteral("卖出");
            }else if (Side == 'G'){
                return QStringLiteral("借入");
            }else if (Side == 'F'){
                return QStringLiteral("出借");
            }else{
                return QStringLiteral("未知类型") + (char)(Side);
            }
        }

        QString sshSideString(uint8_t Side) const
        {
            if (Side == 'B'){
                return QStringLiteral("买单");
            }else if (Side == 'S'){
                return QStringLiteral("卖单");
            }else{
                return QStringLiteral("未知类型") + (char)(Side);
            }
        }
    };

}
