#pragma once

#include "types/s4type.h"

#include <QWidget>
#include <QKeyEvent>
#include <QGraphicsView>
#include <QTabWidget>
#include <QDebug>
#include <QMouseEvent>


namespace S4{
namespace QT{
    

class snapInstrument_tableBasic: public QWidget
{
    Q_OBJECT
public:
    explicit snapInstrument_tableBasic(QWidget *parent = nullptr):
        QWidget(parent)
    {};

    void mouseMoveEvent(QMouseEvent* )
    {
        //qDebug() << "snapInstrument_tableBasic " << hasMouseTracking() << " " << event->pos().x() << ", " << event->pos().y();
    }
    
public slots:

    void addBasic(){}   //TODO
    
private:


private:

};


} // namespace QT
} // namespace S4
