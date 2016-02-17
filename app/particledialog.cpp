#include "particledialog.h"

#include <QColorDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>

void ColorButton::init()
{
    colorPrompt = "Pick your color";
    setColor(QColor("white"));
    connect(this, SIGNAL(clicked()), this, SLOT(changeColor()));
}

void ColorButton::changeColor()
{
    QColor newColor = QColorDialog::getColor(myColor, this, colorPrompt);
    if(newColor.isValid())
        setColor(newColor);
}

QColor ColorButton::getColor()
{
    return myColor;
}

void ColorButton::setColor(QColor newColor)
{
    myColor = newColor;
    const QString COLOR_STYLE("QPushButton { background-color : %1; color : %2; }");
    int backgroundDelta = (myColor.red() * 0.299) + (myColor.green() * 0.587) + (myColor.blue() * 0.114);
    QColor textColor = QColor((255- backgroundDelta < 105) ? Qt::black : Qt::white);
    setStyleSheet(COLOR_STYLE.arg(myColor.name()).arg(textColor.name()));
    setText(myColor.name());
    emit colorChanged();
}

void ColorButton::setPrompt(QString text)
{
    colorPrompt = text;
}

ParticleDialog::ParticleDialog(QWidget *parent) :
    QDialog(parent)
{
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *labelColor;
    QLabel *labelRadius;
    QLabel *labelMass;
    QDialogButtonBox *buttonBox;

    resize(278, 225);
    setWindowIcon(QIcon(":qss_icons/rc/add_particles_64"));

    groupBox = new QGroupBox(this);
    formLayout = new QFormLayout(groupBox);
    labelAmount = new QLabel(groupBox);
    formLayout->setWidget(0, QFormLayout::LabelRole, labelAmount);
    labelColor = new QLabel(groupBox);
    formLayout->setWidget(1, QFormLayout::LabelRole, labelColor);
    labelRadius = new QLabel(groupBox);
    formLayout->setWidget(2, QFormLayout::LabelRole, labelRadius);
    labelMass = new QLabel(groupBox);
    formLayout->setWidget(3, QFormLayout::LabelRole, labelMass);
    labelKine = new QLabel(groupBox);
    formLayout->setWidget(4, QFormLayout::LabelRole, labelKine);

    spinBoxDensity = new QDoubleSpinBox(groupBox);
    spinBoxDensity->setMinimum(0.01);
    spinBoxDensity->setMaximum(100);
    spinBoxDensity->setValue(0.24); // density needed for a sphere of radius 1 to weight exactly 1
    formLayout->setWidget(3, QFormLayout::FieldRole, spinBoxDensity);
    spinBoxAmount = new QSpinBox(groupBox);
    spinBoxAmount->setMinimum(1);
    spinBoxAmount->setMaximum(100000);
	spinBoxAmount->setValue(50);
    formLayout->setWidget(0, QFormLayout::FieldRole, spinBoxAmount);
    spinBoxRadius = new QDoubleSpinBox(groupBox);
    spinBoxRadius->setMinimum(0.01);
    spinBoxRadius->setMaximum(100);
    spinBoxRadius->setValue(0.5);
    formLayout->setWidget(2, QFormLayout::FieldRole, spinBoxRadius);
    colorButton = new ColorButton(groupBox);
    formLayout->setWidget(1, QFormLayout::FieldRole, colorButton);
    colorButton->setColor(QColor(59, 128, 64));
    checkBoxKinamatic = new QCheckBox(groupBox);
    formLayout->setWidget(4, QFormLayout::FieldRole, checkBoxKinamatic);

    verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(groupBox);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    verticalLayout->addWidget(buttonBox);

    setWindowTitle("Adding a particle group");
    groupBox->setTitle("Particle group properties");
    labelAmount->setText("Amount");
    labelColor->setText("Color");
    labelRadius->setText("Radius");
    labelMass->setText("Density");
    labelKine->setText("Kinematic");

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveProperties()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void ParticleDialog::saveProperties()
{
    property.amount = spinBoxAmount->value();
    property.radius = spinBoxRadius->value();
    float r3 = property.radius * property.radius * property.radius;
    property.mass = spinBoxDensity->value()*(4.18879020479*r3);
    QColor c = colorButton->getColor();
    property.r = c.redF();
    property.g = c.greenF();
    property.b = c.blueF();
    property.isKinematic = checkBoxKinamatic->isChecked();
    accept();
}

void ParticleDialog::meshVersion()
{
    labelAmount->hide();
    spinBoxAmount->hide();
    labelKine->hide();
    checkBoxKinamatic->hide();
}
