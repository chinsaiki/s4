﻿#include "qt_Kviewer/s4Kinstrument.h"
#include <QGridLayout>

namespace S4{
namespace QT{
    
Kinstrument::Kinstrument(QWidget *parent) :
    QWidget(parent)
{
	int i;
	_K_tab = new QTabWidget(this);
	Kinstrument_scene* _K_scene = new Kinstrument_scene(this);
	Kinstrument_view* _K_view = new Kinstrument_view(_K_scene, this);
	i = _K_tab->addTab(_K_view, "day");
	_K_tab->setCurrentIndex(i);


    _indicator_tab = new QTabWidget(this);
	Kinstrument_scene* ind_scene = new Kinstrument_scene(this);
	Kinstrument_view* ind_view = new Kinstrument_view(ind_scene, this);
	i = _indicator_tab->addTab(ind_view, "ind");
	_indicator_tab->setCurrentIndex(i);

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
	pLayout->addWidget(_K_tab, 0, 0, 4, 6);		//3x6 row x col
	pLayout->addWidget(_cyc_tab, 0, 6, 4, 1);		//3x1
	pLayout->addWidget(_indicator_tab, 4, 0, 2, 6);	//1x6
	pLayout->addWidget(_basic_tab, 4, 6, 2, 1);		//1x1

	_cyc_tab->setMaximumWidth(400);
	_basic_tab->setMaximumWidth(400);

	pLayout->setRowStretch(0, 5);			//
	pLayout->setRowStretch(4, 2);			//
	pLayout->setColumnStretch(0, 4);			//0列的拉伸系数
	pLayout->setColumnStretch(6, 2);			//6列的拉伸系数 (0=不拉伸)

	setLayout(pLayout);
}

} // namespace QT
} // namespace S4
