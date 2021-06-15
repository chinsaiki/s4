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

    class snapTableModel_exec : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<size_t, QVariant> mapTimeout;

        enum class dataType_t{
            BidApplSeqNum,
            OfferApplSeqNum,
            LastPx,
            LastQty,
            ExecType,
            TransactTime,
        };

        std::vector<dataType_t> _title = {
            dataType_t::BidApplSeqNum,
            dataType_t::OfferApplSeqNum,
            dataType_t::LastPx,
            dataType_t::LastQty,
            dataType_t::ExecType,
            dataType_t::TransactTime,
        };

        enum class unionType_t{
            CANCEL,
            EXECUTION,
            BID,
            ASK,
            UNKNOWN
        };

        struct unionExec_t{
            int64_t         BidApplSeqNum;
            int64_t         OfferApplSeqNum;
            float           LastPx;
            int64_t         LastQty;
            QString         ExecType;
            QString        TransactTime;
            enum class unionType_t unionType;
        };

        QList<unionExec_t> _data;

        QTimeLine* _timeLine;
    public:
        snapTableModel_exec(QObject *parent = {}) : QAbstractTableModel{parent}
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
            const auto& exec = _data[index.row()];
            switch (index.column()) {
            case 0: return QVariant::fromValue(exec.BidApplSeqNum);
            case 1: return QVariant::fromValue(exec.OfferApplSeqNum);
            case 2: return priceString(exec.LastPx).c_str();
            case 3: return QVariant::fromValue(exec.LastQty);
            case 4: return exec.ExecType;
            case 5: return QVariant::fromValue(exec.TransactTime);
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
            size_t sbe_size = l2data->size();
            if (sbe_size < sizeof(SBE_SSH_header_t)){
                return;
            }
            const SBE_SSH_header_t* pH = (SBE_SSH_header_t*)l2data->get();
            unionExec_t data;

            if (pH->SecurityIDSource == 101 && pH->MsgType == __MsgType_SSH_EXECUTION__ && pH->MsgLen == sizeof(SBE_SSH_exe_t)){
                const SBE_SSH_exe_t* pExec = (SBE_SSH_exe_t*)l2data->get();
                data.BidApplSeqNum = pExec->TradeBuyNo;
                data.OfferApplSeqNum = pExec->TradeSellNo;
                data.LastPx = L2_iPrice_tick_to_fPrice(pExec->LastPx);
                data.LastQty = L2_Qty_to_hand(pExec->LastQty);
                data.ExecType = sshExecTypeString(pExec->TradeBSFlag);
                data.TransactTime = ssh_L2_timeString(pExec->TradeTime).c_str();
                
                if (pExec->TradeBSFlag == 'B'){
                    data.unionType = unionType_t::BID;
                }else if (pExec->TradeBSFlag == 'S'){
                    data.unionType = unionType_t::ASK;
                }else{
                    data.unionType = unionType_t::UNKNOWN;
                }
            }else 
            if (pH->SecurityIDSource == 102 && pH->MsgType == __MsgType_SSZ_EXECUTION__ && pH->MsgLen == sizeof(SBE_SSZ_exe_t)){
                const SBE_SSZ_exe_t* pExec = (SBE_SSZ_exe_t*)l2data->get();
                data.BidApplSeqNum = pExec->BidApplSeqNum;
                data.OfferApplSeqNum = pExec->OfferApplSeqNum;
                data.LastPx = L2_iPrice_tick_to_fPrice(pExec->LastPx);
                data.LastQty = L2_Qty_to_hand(pExec->LastQty);
                data.ExecType = sszExecTypeString(pExec->ExecType);
                data.TransactTime = ssz_L2_timeString(pExec->TransactTime).c_str();
                if (pExec->ExecType == 'F'){
                    data.unionType = unionType_t::EXECUTION;
                }else if (pExec->ExecType == '4'){
                    data.unionType = unionType_t::CANCEL;
                }else{
                    data.unionType = unionType_t::UNKNOWN;
                }
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
                if (mapTimeout[i-1].toDateTime().msecsTo(now) <= itemFormatDelegate::update_scope && i-1 < 200){
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
            if (index.column() != 4){   //类型
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
                const auto& exec = _data[index.row()];
                if (exec.unionType == unionType_t::BID){
                    return  QColor(255, 128, 128);
                }else if (exec.unionType == unionType_t::ASK){
                    return  QColor(128, 255, 128);
                }else if (exec.unionType == unionType_t::EXECUTION){
                    return  QColor(128, 128, 255);
                }else if (exec.unionType == unionType_t::CANCEL){
                    return  QColor(128, 128, 128);
                }else{
                    return  QColor(255, 255, 128);
                }
            }
        }

        QString typeString(dataType_t t) const
        {
            switch (t)
            {
            case dataType_t::BidApplSeqNum: return QStringLiteral("买方订单号");
            case dataType_t::OfferApplSeqNum: return QStringLiteral("卖方订单号");
            case dataType_t::LastPx: return QStringLiteral("价格");
            case dataType_t::LastQty: return QStringLiteral("手数");
            case dataType_t::ExecType: return QStringLiteral("类型");
            case dataType_t::TransactTime: return QStringLiteral("时间戳");
            default:return "";
            }
        }

        QString sszExecTypeString(uint8_t ExecType) const
        {
            if (ExecType == 'F'){
                return QStringLiteral("成交");
            }else if (ExecType == '4'){
                return QStringLiteral("撤单");
            }else{
                return QStringLiteral("未知类型") + (char)(ExecType);
            }
        }

        QString sshExecTypeString(uint8_t ExecType) const
        {
            if (ExecType == 'B'){
                return QStringLiteral("外盘");
            }else if (ExecType == 'S'){
                return QStringLiteral("内盘");
            }else{
                return QStringLiteral("未知类型") + QString::number(ExecType);
            }
        }


    };

}
