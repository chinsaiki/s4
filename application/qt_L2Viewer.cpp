#include "qt_L2Viewer/s4L2Viewer.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    S4::QT::L2Viewer w;
    w.show();

    return a.exec();
}
