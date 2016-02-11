#include "mainwindow.h"
#include <QApplication>
#include <qtutils.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QString css = QtUtils::fileToString(":qdarkstyle/style.qss");
    if (!css.isEmpty())
        qApp->setStyleSheet(css);

    w.show();
    return a.exec();
}
