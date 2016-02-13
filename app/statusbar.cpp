#include "statusbar.h"

#include <QLabel>

StatusBar::StatusBar(QWidget* parent) :
    QStatusBar(parent)
{
    particleCountLabel = new QLabel(this);
    constraintCountLabel = new QLabel(this);
    renderingSpeedLabel = new QLabel(this);
    physicsSpeedLabel = new QLabel(this);

    addPermanentWidget(renderingSpeedLabel);
    addPermanentWidget(physicsSpeedLabel);
    addPermanentWidget(particleCountLabel);
    addPermanentWidget(constraintCountLabel);
}
