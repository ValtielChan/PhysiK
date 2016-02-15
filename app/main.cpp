#include "mainwindow.h"
#include <QApplication>
#include <qtutils.h>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSurfaceFormat format;
    format.setMinorVersion(3);
    format.setMajorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow w;

    QString css = QtUtils::fileToString(":qdarkstyle/style.qss");
    if (!css.isEmpty())
        qApp->setStyleSheet(css);

    w.show();
    return a.exec();
}
