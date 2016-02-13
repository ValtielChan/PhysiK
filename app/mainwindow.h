#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class StatusBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    StatusBar *statusBar;
};

#endif // MAINWINDOW_H
