//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/SensorsManagerWidget.h"

//----------------------------------------------------------------------------

SensorsManagerWidget::SensorsManagerWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database)
{
    QGroupBox *groupBoxSensors = new QGroupBox(tr("Sets of sensors: "), this);

    m_buttonAdd    = new QPushButton(tr("Add set"), groupBoxSensors);
//    m_buttonEdit   = new QPushButton(tr("Edit set"), groupBoxSensors);
    m_buttonDelete = new QPushButton(tr("Delete set"), groupBoxSensors);

    QVBoxLayout *layoutGroupBoxSensors = new QVBoxLayout;
    layoutGroupBoxSensors->setAlignment(Qt::AlignHCenter);
    layoutGroupBoxSensors->addWidget(m_buttonAdd);
//    layoutGroupBoxSensors->addWidget(m_buttonEdit);
    layoutGroupBoxSensors->addWidget(m_buttonDelete);
    groupBoxSensors->setLayout(layoutGroupBoxSensors);

    QHBoxLayout *layoutWidget = new QHBoxLayout;
    layoutWidget->setAlignment(Qt::AlignHCenter);
    layoutWidget->addWidget(groupBoxSensors);

    setLayout(layoutWidget);

    connect(m_buttonAdd, SIGNAL(clicked()), this, SLOT(addSetOfSensorsDialog()));
//    connect(m_buttonEdit, SIGNAL(clicked()), this, SLOT(editSetOfSensorsDialog()));
    connect(m_buttonDelete, SIGNAL(clicked()), this, SLOT(deleteSetOfSensorsDialog()));

    connect(this, SIGNAL(connexionStatusChanged()), this, SLOT(enableWidgets()));

    m_connexionEnabled = false;
    emit connexionStatusChanged();
}

SensorsManagerWidget::~SensorsManagerWidget()
{

}

//----------------------------------------------------------------------------

void SensorsManagerWidget::setConnexionEnabled(bool connexionEnabled)
{
    m_connexionEnabled = connexionEnabled;

    emit connexionStatusChanged();
}

void SensorsManagerWidget::enableWidgets()
{
    m_buttonAdd->setEnabled(m_connexionEnabled);
//    m_buttonEdit->setEnabled(m_connexionEnabled);
    m_buttonDelete->setEnabled(m_connexionEnabled);
}    

void SensorsManagerWidget::displaySensorWidgets(int numberOfSensors)
{
    if(numberOfSensors > m_sensors->size())
    {
        for(int i = m_sensors->size(); i < numberOfSensors; i++)
        {
            m_sensors->push_back(new SensorWidget);
            m_layoutSensors->addWidget(m_sensors->last());
        }
    }
    else if(numberOfSensors <  m_sensors->size())
    {
        for(int i = m_sensors->size(); i > numberOfSensors; i--)
        {
            m_layoutSensors->removeWidget(m_sensors->last());
            delete m_sensors->last();
            m_sensors->pop_back();
        }
    }
}

//----------------------------------------------------------------------------

void SensorsManagerWidget::addSetOfSensorsDialog()
{
                m_name        = new QLineEdit();
    QLabel      *nameLabel    = new QLabel(tr("Name of the set: "));
    QHBoxLayout *layoutName   = new QHBoxLayout;

    layoutName->addWidget(nameLabel);
    layoutName->addWidget(m_name);


                m_numberOfSensors      = new QSpinBox;
    QLabel      *numberOfSensorsLabel  = new QLabel(tr("Number of sensors: "));
    QHBoxLayout *layoutNumberOfSensors = new QHBoxLayout;

    layoutNumberOfSensors->addWidget(numberOfSensorsLabel);
    layoutNumberOfSensors->addWidget(m_numberOfSensors);

                m_sensors         = new QVector<SensorWidget*>;

    QLabel      *sensorName       = new QLabel(tr("Name"));
    QLabel      *sensorX          = new QLabel(tr("X (cm)"));
    QLabel      *sensorY          = new QLabel(tr("Y (cm)"));
    QLabel      *sensorZ          = new QLabel(tr("Z (cm)"));

    QHBoxLayout *layoutLabels     = new QHBoxLayout;

    layoutLabels->addWidget(sensorName);
    layoutLabels->addWidget(sensorX);
    layoutLabels->addWidget(sensorY);
    layoutLabels->addWidget(sensorZ);

                m_layoutSensors   = new QVBoxLayout;

    m_layoutSensors->addLayout(layoutLabels);

                m_buttonDialogAdd = new QPushButton(tr("Add"));
    QPushButton *buttonCancel     = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons    = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogAdd);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutName);
    layoutDialog->addLayout(layoutNumberOfSensors);
    layoutDialog->addLayout(m_layoutSensors);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);
    
    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogAdd, SIGNAL(clicked()), this, SLOT(addSetOfSensors()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_buttonDialogAdd->setEnabled(false);
    connect(m_name, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_numberOfSensors, SIGNAL(valueChanged(int)), this, SLOT(displaySensorWidgets(int)));

    dialogBox->exec();
}

void SensorsManagerWidget::enableButtonDialogAdd()
{
    if(m_name->text().isEmpty())
        m_buttonDialogAdd->setEnabled(false);
    else
        m_buttonDialogAdd->setEnabled(true);
}

//void SensorsManagerWidget::editSetOfSensorsDialog()
//{
//                m_names        = new QComboBox();
//    QLabel      *namesLabel    = new QLabel(tr("Sets: "));
//    QHBoxLayout *layoutNames   = new QHBoxLayout;
//
//    layoutNames->addWidget(namesLabel);
//    layoutNames->addWidget(m_names);
//
//                m_buttonDialogEdit = new QPushButton(tr("Edit"));
//    QPushButton *buttonCancel      = new QPushButton(tr("Cancel"));
//    QHBoxLayout *layoutButtons     = new QHBoxLayout;
//
//    layoutButtons->addWidget(m_buttonDialogEdit);
//    layoutButtons->addWidget(buttonCancel);
//   
//    QVBoxLayout *layoutDialog = new QVBoxLayout;
//    layoutDialog->addLayout(layoutNames);
//    layoutDialog->addLayout(layoutButtons);
//
//    dialogBox = new QDialog();
//    dialogBox->setLayout(layoutDialog);
//
//    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
//    connect(m_buttonDialogEdit, SIGNAL(clicked()), this, SLOT(editSetOfSensors()));
//    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));
//
//    m_names->addItem(tr("Select a set..."));
//    QSqlQuery query;
//    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Sets_of_sensors")))
//    {
//        while(query.next())
//        {
//            m_names->addItem(query.value(0).toString());
//        }
//    }
//    else
//        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
//
//    m_buttonDialogEdit->setEnabled(false);
//    connect(m_names, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
//
//    connect(m_names, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSetOfSensors(QString)));
//
//    dialogBox->exec();
//}
//
//void SensorsManagerWidget::enableButtonDialogEdit()
//{
//    if(m_names->currentIndex() == 0)
//        m_buttonDialogEdit->setEnabled(false);
//    else
//        m_buttonDialogEdit->setEnabled(true);
//}
//
//void SensorsManagerWidget::updateSetOfSensors(QString setName)
//{
////    QSqlQuery query;
////    if(query.exec(QString("SELECT x,y,z,insulation_thickness FROM %1 WHERE name = \"%2\"").arg("Sets_of_sensors").arg(setName)))
////    {
////        while(query.next())
////        {
////            unsigned short index = 0;
////            m_x->setText(query.value(index++).toString());
////            m_y->setText(query.value(index++).toString());
////            m_z->setText(query.value(index++).toString());
////            m_insulationThickness->setText(query.value(index++).toString());
////        }
////    }
////    else
////        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
//}

void SensorsManagerWidget::deleteSetOfSensorsDialog()
{
                m_names        = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Sets: "));
    QHBoxLayout *layoutNames   = new QHBoxLayout;

    layoutNames->addWidget(namesLabel);
    layoutNames->addWidget(m_names);

                m_buttonDialogDelete = new QPushButton(tr("Delete"));
    QPushButton *buttonCancel        = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons       = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogDelete);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNames);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogDelete, SIGNAL(clicked()), this, SLOT(deleteSetOfSensors()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_names->addItem(tr("Select a set..."));
    QSqlQuery query;
    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Sets_of_sensors")))
    {
        while(query.next())
        {
            m_names->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_buttonDialogDelete->setEnabled(false);
    connect(m_names, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogDelete()));

    dialogBox->exec();
}

void SensorsManagerWidget::enableButtonDialogDelete()
{
    if(m_names->currentIndex() == 0)
        m_buttonDialogDelete->setEnabled(false);
    else
        m_buttonDialogDelete->setEnabled(true);
}

//----------------------------------------------------------------------------

void SensorsManagerWidget::addSetOfSensors()
{
//    QSqlQuery query;
//
//    if(query.exec(QString("SELECT * FROM %1 WHERE name = \"%2\"").arg("Test_benches").arg(m_name->text())))
//    {
//        if(query.size() > 0)
//            QMessageBox::critical(dialogBox, tr("Error"), tr("A test bench with this name is already existing."));
//        else
//        {
//            if(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, %5, %6)").arg("Test_benches").arg(m_name->text()).arg(m_x->text().toInt()).arg(m_y->text().toInt()).arg(m_z->text().toInt()).arg(m_insulationThickness->text().toInt())))
//            {
//                QMessageBox::information(this, tr("Result"), tr("Test bench succesfully added"));
//                emit closeDialog();
//            }
//            else
//                QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of the test bench in the database failed."));
//        }
//    }
//    else
//        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

//void SensorsManagerWidget::editSetOfSensors()
//{
////    QSqlQuery query;
////
////    if(query.exec(QString("UPDATE %1 SET x=%2, y=%3, z=%4, insulation_thickness=%5 WHERE name=\"%6\"").arg("Test_benches").arg(m_x->text().toInt()).arg(m_y->text().toInt()).arg(m_z->text().toInt()).arg(m_insulationThickness->text().toInt()).arg(m_names->currentText())))
////    {
////        QMessageBox::information(this, tr("Result"), tr("Test bench succesfully edited"));
////        emit closeDialog();
////    }
////    else
////        QMessageBox::critical(dialogBox, tr("Error"), tr("Edition of the test bench in the database failed."));
//}

void SensorsManagerWidget::deleteSetOfSensors()
{
    QSqlQuery query;

    if(query.exec(QString("DELETE FROM %1 WHERE name=\"%2\"").arg("Sets_of_sensors").arg(m_names->currentText())))
    {
        QMessageBox::information(this, tr("Result"), tr("Set of sensors succesfully deleted"));
        emit closeDialog();
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Deletion of the set of sensors in the database failed."));

}

//----------------------------------------------------------------------------
