#ifndef STATSPBD_H
#define STATSPBD_H

#include <QWidget>

namespace Ui {
class StatsPBD;
}

class StatsPBD : public QWidget
{
    Q_OBJECT

public:
    explicit StatsPBD(QWidget *parent = 0);
    ~StatsPBD();

private:
    Ui::StatsPBD *ui;

    QTimer *refreshFPSTimer;

    int nbRefreshes;
    double renderingTime;
    double physicsTime;

private slots:
    void updateSpeedLabel();

public slots:
    void updateSpeed(double physicsTime, double renderingTime);
    void updateTimeRate(float);

signals:
    void changedTimeRate(int);
    void changedSolverIterations(int);
};

#endif // STATSPBD_H
