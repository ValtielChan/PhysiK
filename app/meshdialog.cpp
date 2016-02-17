#include "meshdialog.h"

#include <QGroupBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QFormLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QCoreApplication>

#include "wavefrontmesh.h"

MeshDialog::MeshDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUI();
}

void MeshDialog::setupUI()
{
    properties.isRigid = true;
    toParticles = false;
    noPhysics = false;
    properties.isKinematic = true;
    properties.mass = 2;

    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout;
    QGroupBox *groupBox;

    resize(391, 294);
    verticalLayout = new QVBoxLayout(this);
    groupBox_2 = new QGroupBox(this);
    horizontalLayout = new QHBoxLayout(groupBox_2);
    lineEdit = new QLineEdit(groupBox_2);
    lineEdit->setReadOnly(true);

    horizontalLayout->addWidget(lineEdit);

    pushButton = new QPushButton(groupBox_2);

    horizontalLayout->addWidget(pushButton);

    verticalLayout->addWidget(groupBox_2);

    groupBox = new QGroupBox(this);
    formLayout = new QFormLayout(groupBox);
    label = new QLabel(groupBox);

    formLayout->setWidget(3, QFormLayout::LabelRole, label);

    checkBox = new QCheckBox(groupBox);
    checkBox->setChecked(true);

    formLayout->setWidget(3, QFormLayout::FieldRole, checkBox);

    label_2 = new QLabel(groupBox);

    formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

    label_3 = new QLabel(groupBox);

    formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

    doubleSpinBox = new QDoubleSpinBox(groupBox);
    doubleSpinBox->setMinimum(0.01);
    doubleSpinBox->setMaximum(100);
    doubleSpinBox->setValue(2);

    formLayout->setWidget(2, QFormLayout::FieldRole, doubleSpinBox);

    comboBox = new QComboBox(groupBox);

    formLayout->setWidget(1, QFormLayout::FieldRole, comboBox);

    verticalLayout->addWidget(groupBox);

    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    verticalLayout->addWidget(buttonBox);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setWindowTitle("Load Mesh");
    groupBox_2->setTitle("Mesh file");
    lineEdit->setText(QString());
    pushButton->setText("Pick File");
    groupBox->setTitle("Mesh Properties");
    label->setText("Kinematic");
    checkBox->setText(QString());
    label_2->setText("Type");
    label_3->setText("Mass");
    comboBox->clear();
    comboBox->insertItems(0, QStringList() << "RigidBody" << "Particle Group" << "SoftBody" << "No physics");

    connect(pushButton, SIGNAL(pressed()), this, SLOT(pickFile()));
    connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(setKinematic(bool)));
    connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(setMass(double)));
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setType(int)));
}

void MeshDialog::pickFile()
{
    WavefrontMesh meshLoader;
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Wavefront mesh"),
                                                    QCoreApplication::applicationDirPath().append("/../assets"),
                                                    tr("Wavefront Object (*.obj)"));
    if(!filename.isNull())
    {
        meshes = meshLoader.loadMesh(filename);
        if(meshes.size() > 0)
        {
            lineEdit->setText(filename);
            buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
        return;
    }
    meshes.clear();
    lineEdit->setText(QString());
    buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

void MeshDialog::setKinematic(bool isKinematic)
{
    properties.isKinematic = isKinematic;
}

void MeshDialog::setMass(double mass)
{
    properties.mass = mass;
}

void MeshDialog::setType(int type)
{
    properties.isRigid = true;
    toParticles = false;
    noPhysics = false;

    switch(type)
    {
    case 0 : // RigidBody
    break;
    case 1 : // Particles
        toParticles = true;
    break;
    case 2 : // SoftBody
        properties.isRigid = false;
    break;
    case 3 : // Background model
        noPhysics = true;
    break;
    }
}
