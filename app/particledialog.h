#ifndef PARTICLEDIALOG_H
#define PARTICLEDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QSpinBox>
#include <QCheckBox>
#include "scenemanager.h"

class QSpinBox;
class QLabel;

class ColorButton : public QPushButton
{
    Q_OBJECT

private:
    QColor myColor;
    QString colorPrompt;

    void init();

private slots:
    void changeColor();

public:
    ColorButton(QWidget* parent = 0) : QPushButton(parent) {init();}
    QColor getColor();
    void setColor(QColor newColor);
    void setPrompt(QString text);

signals:
    void colorChanged();
};

class ParticleDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ParticleDialog(QWidget *parent = 0);

        ParticleProperties getParticleProperties() {return property;}
        void meshVersion();

    private:
        ParticleProperties property;

        QDoubleSpinBox *spinBoxDensity;
        QSpinBox *spinBoxAmount;
        QDoubleSpinBox *spinBoxRadius;
        QCheckBox *checkBoxKinamatic;
        ColorButton *colorButton;
        QLabel *labelAmount;
        QLabel *labelKine;

    private slots:
        void saveProperties();
};

#endif // PARTICLEDIALOG_H
