#include "mainwindow.h"
#include <QApplication>
#include <qtutils.h>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{

    QSurfaceFormat format;
    format.setMinorVersion(3);
    format.setMajorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);

    MainWindow w;

    QString css = QtUtils::fileToString(":qdarkstyle/style.qss");
    if (!css.isEmpty())
        qApp->setStyleSheet(css);

    w.show();
    return a.exec();
}
