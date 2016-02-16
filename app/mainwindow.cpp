#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_A :
            ui->actionParticles->trigger();
        break;
        case Qt::Key_Z :
            ui->actionRigidBody->trigger();
        break;
        case Qt::Key_E :
            ui->actionReset_Scene->trigger();
        break;
        case Qt::Key_R :
            ui->actionReset_Camera->trigger();
        break;
        case Qt::Key_Space :
            ui->actionPause->trigger();
        break;
        case Qt::Key_S:
            ui->statsWidget->updateTimeRate(0.5f);
        break;
        case Qt::Key_D:
            ui->statsWidget->updateTimeRate(2.f);
        break;
    }
}
