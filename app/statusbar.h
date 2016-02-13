#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class QLabel;

class StatusBar : public QStatusBar
{
    private:
        Q_OBJECT

        QLabel *particleCountLabel;
        QLabel *constraintCountLabel;
        QLabel *renderingSpeedLabel;
        QLabel *physicsSpeedLabel;

    public:
        StatusBar(QWidget* parent = 0);
};

#endif // STATUSBAR_H
