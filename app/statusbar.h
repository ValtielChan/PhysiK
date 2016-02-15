#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <QStatusBar>

class QLabel;
class QTimer;

class StatusBar : public QStatusBar
{
    private:
        Q_OBJECT

        QTimer *refreshFPSTimer;

        QLabel *particleCountLabel;
        QLabel *constraintCountLabel;

        QLabel *FPSLabel;
        QLabel *physicsPerfLabel;
        QLabel *renderingPerfLabel;
        int nbRefreshes;
        double renderingTime;
        double physicsTime;

    public:
        StatusBar(QWidget* parent = 0);

    private slots:
        void updateSpeedLabel();

    public slots:
        void updateSpeed(double physicsTime, double renderingTime);

};

#endif // STATUSBAR_H
