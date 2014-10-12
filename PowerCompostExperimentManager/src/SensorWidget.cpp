//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/SensorWidget.h"

//----------------------------------------------------------------------------

SensorWidget::SensorWidget(QWidget *parent)
    : QWidget(parent)
{
    m_name  = new QLineEdit();
    m_x     = new QLineEdit();
    m_y     = new QLineEdit();
    m_z     = new QLineEdit();
    
    QHBoxLayout *layoutSensor = new QHBoxLayout;
    layoutSensor->addWidget(m_name);
    layoutSensor->addWidget(m_x);
    layoutSensor->addWidget(m_y);
    layoutSensor->addWidget(m_z);

    setLayout(layoutSensor);

    connect(m_name, SIGNAL(textChanged(QString)), this, SLOT(informTextChanged()));
    connect(m_x, SIGNAL(textChanged(QString)), this, SLOT(informTextChanged()));
    connect(m_y, SIGNAL(textChanged(QString)), this, SLOT(informTextChanged()));
    connect(m_z, SIGNAL(textChanged(QString)), this, SLOT(informTextChanged()));

}

SensorWidget::~SensorWidget()
{

}

//----------------------------------------------------------------------------

QString SensorWidget::getName()
{
    return m_name->text();
}

int SensorWidget::getX()
{
    return m_x->text().toInt();
}

int SensorWidget::getY()
{
    return m_y->text().toInt();
}

int SensorWidget::getZ()
{
    return m_z->text().toInt();
}

//----------------------------------------------------------------------------

void SensorWidget::setName(QString name)
{
    m_name->setText(name);
}

void SensorWidget::setX(int value)
{
    m_x->setText(QString::number(value));
}

void SensorWidget::setY(int value)
{
    m_y->setText(QString::number(value));
}

void SensorWidget::setZ(int value)
{
    m_z->setText(QString::number(value));
}

//----------------------------------------------------------------------------

bool SensorWidget::isEmpty()
{
    if(m_name->text().isEmpty() || m_x->text().isEmpty() || m_y->text().isEmpty() || m_z->text().isEmpty())
    {
        return true;
    }
    else
        return false;
}

//----------------------------------------------------------------------------

void SensorWidget::informTextChanged()
{
    emit textChanged();
}

//----------------------------------------------------------------------------
