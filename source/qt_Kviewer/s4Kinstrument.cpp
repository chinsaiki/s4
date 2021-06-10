#include "qt_Kviewer/s4Kinstrument.h"
#include <QGridLayout>

namespace S4{
namespace QT{
    
Kinstrument::Kinstrument(QWidget *parent) :
    QWidget(parent)
{
	this->setMouseTracking(true);
	int i;
	_K_tab = new Kinstrument_Kline_tab(this);

	_indicator_tab = new Kinstrument_indicator_tab(this);

	connect(_K_tab, &Kinstrument_Kline_tab::paint_indicator,
		_indicator_tab, &Kinstrument_indicator_tab::paint);

	connect(_K_tab, SIGNAL(signalViewEvent(std::shared_ptr<view_event>)), _indicator_tab, SLOT(slotViewEvent(std::shared_ptr<view_event>)));

    _cyc_tab = new QTabWidget(this);
	Kinstrument_scene* cyc_scene = new Kinstrument_scene(this);
	Kinstrument_view* cyc_view = new Kinstrument_view(cyc_scene, this);
	i = _cyc_tab->addTab(cyc_view, "cyc");
	_cyc_tab->setCurrentIndex(i);

    _basic_tab = new QTabWidget(this);
	Kinstrument_scene* basic_scene = new Kinstrument_scene(this);
	Kinstrument_view* basic_view = new Kinstrument_view(basic_scene, this);
	i = _basic_tab->addTab(basic_view, "basic");
	_basic_tab->setCurrentIndex(i);
    
	//网格分割
	QGridLayout *pLayout = new QGridLayout();
	pLayout->addWidget(_K_tab, 0, 0, 3, 6);			//3x6 row x col
	pLayout->addWidget(_cyc_tab, 0, 6, 3, 1);		//3x1
	pLayout->addWidget(_indicator_tab, 3, 0, 1, 6);	//1x6
	pLayout->addWidget(_basic_tab, 3, 6, 1, 1);		//1x1

	_cyc_tab->setMaximumWidth(400);
	_basic_tab->setMaximumWidth(400);

	pLayout->setRowStretch(0, 3);			//
	pLayout->setRowStretch(3, 1);			//
	pLayout->setColumnStretch(0, 3);			//0列的拉伸系数
	pLayout->setColumnStretch(6, 1);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);
}


void Kinstrument::setInstrument(const data_panel_t& data_panel){
	_data_panel = std::make_shared<data_panel_t>(data_panel);
	_indicator_tab->setInstrument(_data_panel);

	_K_tab->setInstrument(_data_panel);

}

void Kinstrument::slot_next_trade(int seq)
{
	_K_tab->slot_next_trade(seq);
}

} // namespace QT
} // namespace S4
