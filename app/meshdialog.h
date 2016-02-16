#ifndef MESHDIALOG_H
#define MESHDIALOG_H

#include <QDialog>
#include "bodymesh.h"

class QLineEdit;
class QFormLayout;
class QCheckBox;
class QLabel;
class QDoubleSpinBox;
class QComboBox;
class QDialogButtonBox;

class MeshDialog : public QDialog
{
    Q_OBJECT

    BodyProperties properties;
    std::vector<Mesh*> meshes;
    bool toParticles;
    bool noPhysics;

    void setupUI();

    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QFormLayout *formLayout;
    QLabel *label;
    QCheckBox *checkBox;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QComboBox *comboBox;
    QDialogButtonBox *buttonBox;

public:
    explicit MeshDialog(QWidget *parent = 0);

    BodyProperties getBodyProperties() {return properties;}
    std::vector<Mesh*> getMeshes() {return meshes;}

    bool isNoPhysics() {return noPhysics;}
    bool isToParticles() {return toParticles;}

private slots:
    void pickFile();
    void setKinematic(bool isKinematic);
    void setMass(double mass);
    void setType(int type);
};

#endif // MESHDIALOG_H
