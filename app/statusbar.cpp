#include "statusbar.h"

#include <QLabel>
#include <QTimer>

StatusBar::StatusBar(QWidget* parent) :
    QStatusBar(parent)
{
    refreshFPSTimer = new QTimer(this);

    particleCountLabel = new QLabel(this);
    constraintCountLabel = new QLabel(this);
    FPSLabel = new QLabel(this);
    physicsPerfLabel = new QLabel(this);
    renderingPerfLabel = new QLabel(this);

    addPermanentWidget(physicsPerfLabel);
    addPermanentWidget(renderingPerfLabel);
    addPermanentWidget(FPSLabel);
    //addPermanentWidget(particleCountLabel);
    //addPermanentWidget(constraintCountLabel);

    connect(refreshFPSTimer, SIGNAL(timeout()), this, SLOT(updateSpeedLabel()));
    refreshFPSTimer->start(1000);
}

void StatusBar::updateSpeedLabel()
{
    FPSLabel->setText(QString("FPS (real/possible) = ").append(QString::number(nbRefreshes))
                      .append("/").append(QString::number(nbRefreshes/(physicsTime+renderingTime), 'f', 2)));
    physicsPerfLabel->setText(QString("avg physics time = ").append(QString::number(int(physicsTime*1000/nbRefreshes))).append(" ms"));
    renderingPerfLabel->setText(QString("avg render time = ").append(QString::number(int(renderingTime*1000/nbRefreshes))).append(" ms"));
    nbRefreshes = 0;
    physicsTime = 0;
    renderingTime = 0;
}

void StatusBar::updateSpeed(double physicsT, double renderingT)
{
    ++nbRefreshes;
    physicsTime += physicsT;
    renderingTime += renderingT;
}
