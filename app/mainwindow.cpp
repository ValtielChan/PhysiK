#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "statusbar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    statusBar = new StatusBar(this);
    setStatusBar(statusBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}
