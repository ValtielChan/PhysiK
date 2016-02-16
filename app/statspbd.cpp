#include "statspbd.h"
#include "ui_statspbd.h"
#include <QTimer>

StatsPBD::StatsPBD(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StatsPBD),
    nbRefreshes(0),
    renderingTime(0),
    physicsTime(0)
{
    ui->setupUi(this);

    connect(ui->timeRateSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(changedTimeRate(int)));
    connect(ui->solverIterationsSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(changedSolverIterations(int)));

    refreshFPSTimer = new QTimer(this);
    connect(refreshFPSTimer, SIGNAL(timeout()), this, SLOT(updateSpeedLabel()));
    refreshFPSTimer->start(1000);
}

StatsPBD::~StatsPBD()
{
    delete ui;
}

void StatsPBD::updateSpeedLabel()
{
    ui->fpsLabel->setText(QString::number(nbRefreshes).append("/").append(QString::number(nbRefreshes/(physicsTime+renderingTime), 'f', 2)));
    ui->physicsTimeLabel->setText(QString::number(physicsTime*1000/nbRefreshes, 'f', 2).append(" ms"));
    ui->renderTimeLabel->setText(QString::number(renderingTime*1000/nbRefreshes, 'f', 2).append(" ms"));
    nbRefreshes = 0;
    physicsTime = 0;
    renderingTime = 0;
}

void StatsPBD::updateSpeed(double physicsT, double renderingT)
{
    ++nbRefreshes;
    physicsTime += physicsT;
    renderingTime += renderingT;
}

void StatsPBD::updateTimeRate(float ratio)
{
    ui->timeRateSpinBox->setValue(ratio*ui->timeRateSpinBox->value());
}
