#pragma once

#include "types/s4type.h"
#include "common/s4time.h"
#include "types/s4convertors.h"
#include "qt_common/s4qt_itemFormatDelegate.h"
#include "network/L2_udp_recver_th.h"

#include <QTableView>
#include <QDateTime>
#include <QTimeLine>
#include <QDebug>


namespace S4
{
    class snapTableModel_L2Stats : public QAbstractTableModel
    {
        Q_OBJECT
        QMap<size_t, QVariant> mapTimeout;

        enum class dataType_t{
            recv_frame_cnt,
            recv_heartbeat_cnt,
            recv_instrument_snap_cnt,
            recv_index_snap_cnt,
            recv_order_cnt,
            recv_exec_cnt,
            recv_unknown_cnt,
            last_frame_time_ms,
            live_code_nb,
            live_instrument_snap_cnt,
            live_index_snap_cnt,
            live_order_cnt,
            live_exec_cnt,
        };

        std::vector<dataType_t> _row_names = {
            dataType_t::recv_frame_cnt,
            dataType_t::recv_heartbeat_cnt,
            dataType_t::recv_instrument_snap_cnt,
            dataType_t::recv_index_snap_cnt,
            dataType_t::recv_order_cnt,
            dataType_t::recv_exec_cnt,
            dataType_t::recv_unknown_cnt,
            dataType_t::last_frame_time_ms,
            dataType_t::live_code_nb,
            dataType_t::live_instrument_snap_cnt,
            dataType_t::live_index_snap_cnt,
            dataType_t::live_order_cnt,
            dataType_t::live_exec_cnt,
            };
        std::vector<QVariant> _data;
        QTimeLine* _timeLine;
    public:
        snapTableModel_L2Stats(QObject *parent = {}) : QAbstractTableModel{parent}
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
    public slots:
        void refresh(const struct S4::NW::L2Stats_t &stats)
        {
            std::vector<QVariant> data;
            for (auto& key : _row_names){
                switch(key){
                    case dataType_t::recv_frame_cnt : data.emplace_back((long long)stats.recv_frame_cnt); break;
                    case dataType_t::recv_heartbeat_cnt : data.emplace_back((long long)stats.recv_heartbeat_cnt); break;
                    case dataType_t::recv_instrument_snap_cnt : data.emplace_back((long long)stats.recv_instrument_snap_cnt); break;
                    case dataType_t::recv_index_snap_cnt : data.emplace_back((long long)stats.recv_index_snap_cnt); break;
                    case dataType_t::recv_order_cnt : data.emplace_back((long long)stats.recv_order_cnt); break;
                    case dataType_t::recv_exec_cnt : data.emplace_back((long long)stats.recv_exec_cnt); break;
                    case dataType_t::recv_unknown_cnt : data.emplace_back((long long)stats.recv_unknown_cnt); break;
                    case dataType_t::last_frame_time_ms : data.emplace_back(ms_to_str(stats.last_frame_time_ms).c_str()); break;
                    case dataType_t::live_code_nb : data.emplace_back((long long)stats.live_code_nb); break;
                    case dataType_t::live_instrument_snap_cnt : data.emplace_back((long long)stats.live_instrument_snap_cnt); break;
                    case dataType_t::live_index_snap_cnt : data.emplace_back((long long)stats.live_index_snap_cnt); break;
                    case dataType_t::live_order_cnt : data.emplace_back((long long)stats.live_order_cnt); break;
                    case dataType_t::live_exec_cnt : data.emplace_back((long long)stats.live_exec_cnt); break;
                };
            }

            _refresh(data);
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

        void _refresh(std::vector<QVariant>& data)
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
            case dataType_t::recv_frame_cnt : return QStringLiteral("L2消息");
            case dataType_t::recv_heartbeat_cnt : return QStringLiteral("心跳");
            case dataType_t::recv_instrument_snap_cnt : return QStringLiteral("行情快照");
            case dataType_t::recv_index_snap_cnt : return QStringLiteral("指数快照");
            case dataType_t::recv_order_cnt : return QStringLiteral("逐笔委托");
            case dataType_t::recv_exec_cnt : return QStringLiteral("逐笔成交");
            case dataType_t::recv_unknown_cnt : return QStringLiteral("其它");
            case dataType_t::last_frame_time_ms : return QStringLiteral("最后接收时间");
            case dataType_t::live_code_nb : return QStringLiteral("关注标的数目");
            case dataType_t::live_instrument_snap_cnt : return QStringLiteral("关注标的行情快照");
            case dataType_t::live_index_snap_cnt : return QStringLiteral("关注标的指数快照");
            case dataType_t::live_order_cnt : return QStringLiteral("关注标的逐笔委托");
            case dataType_t::live_exec_cnt : return QStringLiteral("关注标的逐笔成交");
            default:return "";
            }
        }
    };

}
