#pragma once

#include <QTableView>
#include "types/s4type.h"
#include "types/s4convertors.h"
namespace S4
{

    class snapLevels : public QAbstractTableModel
    {
        std::vector<QString> _title = {"Side", "Price", "Volume"};
        const int _side_levels_nb;
        std::vector<orderBookLevel_t> _ask; //0 is best
        std::vector<orderBookLevel_t> _bid; //0 is best
    public:
        snapLevels(int side_levels_nb, QObject *parent = {}) : QAbstractTableModel{parent},
                                                               _side_levels_nb(side_levels_nb)
        {
            _ask.resize(side_levels_nb);
            _bid.resize(side_levels_nb);
        }

        int rowCount(const QModelIndex &) const override { return _side_levels_nb << 1; }
        int columnCount(const QModelIndex &) const override { return (int)_title.size(); }
        QVariant data(const QModelIndex &index, int role) const override
        {
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

            //TODO: 高亮变动
        }
    };

}