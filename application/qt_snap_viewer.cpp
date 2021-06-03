#include "qt_SnapViewer/s4SnapViewer.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    S4::QT::s4SnapViewer w;
    w.show();

    return a.exec();
}
