//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/SensorsManagerWidget.h"

//----------------------------------------------------------------------------

SensorsManagerWidget::SensorsManagerWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database), m_activeWindow(0)
{
    QGroupBox *groupBoxSensors = new QGroupBox(tr("Sets of sensors: "), this);

    m_buttonAdd    = new QPushButton(tr("Add set"), groupBoxSensors);
    m_buttonEdit   = new QPushButton(tr("Edit set"), groupBoxSensors);
    m_buttonDelete = new QPushButton(tr("Delete set"), groupBoxSensors);

    QVBoxLayout *layoutGroupBoxSensors = new QVBoxLayout;
    layoutGroupBoxSensors->setAlignment(Qt::AlignHCenter);
    layoutGroupBoxSensors->addWidget(m_buttonAdd);
    layoutGroupBoxSensors->addWidget(m_buttonEdit);
    layoutGroupBoxSensors->addWidget(m_buttonDelete);
    groupBoxSensors->setLayout(layoutGroupBoxSensors);

    QHBoxLayout *layoutWidget = new QHBoxLayout;
    layoutWidget->setAlignment(Qt::AlignHCenter);
    layoutWidget->addWidget(groupBoxSensors);

    setLayout(layoutWidget);

    connect(m_buttonAdd, SIGNAL(clicked()), this, SLOT(addSetOfSensorsDialog()));
    connect(m_buttonEdit, SIGNAL(clicked()), this, SLOT(editSetOfSensorsDialog()));
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
    m_buttonEdit->setEnabled(m_connexionEnabled);
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

            if(m_activeWindow == 1)
            {
                connect(m_sensors->last(), SIGNAL(textChanged()), this, SLOT(enableButtonDialogAdd()));
            }
            else if(m_activeWindow == 2)
            {
                connect(m_sensors->last(), SIGNAL(textChanged()), this, SLOT(enableButtonDialogEdit()));
            }
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

                m_comments        = new QTextEdit();
    QLabel      *commentsLabel    = new QLabel(tr("Comments: "));
    QHBoxLayout *layoutComments   = new QHBoxLayout;

    layoutComments->addWidget(commentsLabel);
    layoutComments->addWidget(m_comments);

                m_buttonDialogAdd = new QPushButton(tr("Add"));
    QPushButton *buttonCancel     = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons    = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogAdd);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutName);
    layoutDialog->addLayout(layoutNumberOfSensors);
    layoutDialog->addLayout(m_layoutSensors);
    layoutDialog->addLayout(layoutComments);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);
    
    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogAdd, SIGNAL(clicked()), this, SLOT(addSetOfSensors()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_buttonDialogAdd->setEnabled(false);
    connect(m_name, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_numberOfSensors, SIGNAL(valueChanged(int)), this, SLOT(displaySensorWidgets(int)));

    m_activeWindow = 1;

    dialogBox->exec();

//    m_activeWindow = 0;
}

void SensorsManagerWidget::enableButtonDialogAdd()
{
    bool nothingIsEmpty = true;

    if(m_name->text().isEmpty())
        nothingIsEmpty = false;
    else
    {
        for(QVector<SensorWidget*>::iterator it = m_sensors->begin(); it != m_sensors->end(); ++it)
        {
            if((*it)->isEmpty())
                nothingIsEmpty = false;
        }
    }
    
    m_buttonDialogAdd->setEnabled(nothingIsEmpty);
}

void SensorsManagerWidget::editSetOfSensorsDialog()
{
                m_names        = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Sets: "));
    QHBoxLayout *layoutNames   = new QHBoxLayout;

    layoutNames->addWidget(namesLabel);
    layoutNames->addWidget(m_names);

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

                m_comments        = new QTextEdit();
    QLabel      *commentsLabel    = new QLabel(tr("Comments: "));
    QHBoxLayout *layoutComments   = new QHBoxLayout;

    layoutComments->addWidget(commentsLabel);
    layoutComments->addWidget(m_comments);

                m_buttonDialogEdit = new QPushButton(tr("Edit"));
    QPushButton *buttonCancel      = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons     = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogEdit);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNames);
    layoutDialog->addLayout(layoutNumberOfSensors);
    layoutDialog->addLayout(m_layoutSensors);
    layoutDialog->addLayout(layoutComments);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogEdit, SIGNAL(clicked()), this, SLOT(editSetOfSensors()));
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

    m_buttonDialogEdit->setEnabled(false);
    connect(m_names, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_names, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSetOfSensors(QString)));
    connect(m_numberOfSensors, SIGNAL(valueChanged(int)), this, SLOT(displaySensorWidgets(int)));

    m_activeWindow = 2;

    dialogBox->exec();

//    m_activeWindow = 0;
}

void SensorsManagerWidget::enableButtonDialogEdit()
{
    bool nothingIsEmpty = true;

    if(m_names->currentIndex() == 0)
        nothingIsEmpty = false;
    else
    {
        for(QVector<SensorWidget*>::iterator it = m_sensors->begin(); it != m_sensors->end(); ++it)
        {
            if((*it)->isEmpty())
                nothingIsEmpty = false;
        }
    }
    
    m_buttonDialogEdit->setEnabled(nothingIsEmpty);
}

void SensorsManagerWidget::updateSetOfSensors(QString setName)
{
    QSqlQuery query;
    if(query.exec(QString("SELECT nb_sensors,comments FROM %1 WHERE name = \"%2\"").arg("Sets_of_sensors").arg(setName)))
    {
        while(query.next())
        {
            unsigned short index = 0;
            m_numberOfSensors->setValue(query.value(index++).toInt());
            m_comments->setText(query.value(index++).toString());
            
            if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Sets_of_sensors").arg(setName)))
            {
                while(query.next())
                {
                    QString tableId = query.value(0).toString();

                    if(query.exec(QString("SELECT name, x, y, z FROM %1").arg(QString("Set_") + tableId)))
                    {
                        unsigned short index = 0;
                        while(query.next())
                        {
                            (*m_sensors)[index]->setName(query.value(0).toString());
                            (*m_sensors)[index]->setX(query.value(1).toInt());
                            (*m_sensors)[index]->setY(query.value(2).toInt());
                            (*m_sensors)[index]->setZ(query.value(3).toInt());
                            index++;
                        }
                    }
                    else
                        QMessageBox::critical(dialogBox, tr("Error"), tr("Selection of the set in the database failed."));
                }
            }
            else
                QMessageBox::critical(dialogBox, tr("Error"), tr("Selection of the set id in the database failed."));
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

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
    QSqlQuery query;

    if(query.exec(QString("SELECT * FROM %1 WHERE name = \"%2\"").arg("Sets_of_sensors").arg(m_name->text())))
    {
        if(query.size() > 0)
            QMessageBox::critical(dialogBox, tr("Error"), tr("A set of sensors with this name is already existing."));
        else
        {
            if(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, \"%4\")").arg("Sets_of_sensors").arg(m_name->text()).arg(m_numberOfSensors->text().toInt()).arg(m_comments->toPlainText())))
            {
                if(query.exec(QString("SELECT MAX(id) FROM Sets_of_sensors")))
                {
                    while(query.next())
                    {
                        QString tableId = query.value(0).toString();

                        if(query.exec(QString("CREATE TABLE %1 LIKE Set_model").arg(QString("Set_") + tableId))) // PLACEHOLDER: arg
                        {
                            for(QVector<SensorWidget*>::iterator sensor = m_sensors->begin(); sensor != m_sensors->end(); ++sensor)
                            {
                                if(!(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, %5)").arg(QString("Set_") + tableId).arg((*sensor)->getName()).arg((*sensor)->getX()).arg((*sensor)->getY()).arg((*sensor)->getZ())))) // PLACEHOLDER: arg
                                    QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of a sensor failed."));
                            }
                            QMessageBox::information(this, tr("Result"), tr("Set of sensors succesfully added"));
                            emit closeDialog();
                        }
                        else
                            QMessageBox::critical(dialogBox, tr("Error"), tr("Creation of the set in the database failed."));
                    }
                }
                else
                    QMessageBox::critical(dialogBox, tr("Error"), tr("Selection of the set id in the database failed."));
           }
            else
                QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of the set in the database failed."));
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void SensorsManagerWidget::editSetOfSensors()
{
    QSqlQuery query;

    if(query.exec(QString("UPDATE %1 SET nb_sensors=%2, comments=\"%3\" WHERE name=\"%4\"").arg("Sets_of_sensors").arg(m_numberOfSensors->text().toInt()).arg(m_comments->toPlainText()).arg(m_names->currentText())))
    {
        if(query.exec(QString("SELECT id FROM Sets_of_sensors WHERE name=\"%1\"").arg(m_names->currentText())))
        {
            while(query.next())
            {
                QSqlQuery queryUpdate;

                QString tableId = query.value(0).toString();

                for(QVector<SensorWidget*>::iterator sensor = m_sensors->begin(); sensor != m_sensors->end(); ++sensor)
                {
                    if(!(queryUpdate.exec(QString("UPDATE %1 SET name=\"%2\", x=%3, y=%4, z=%5 WHERE id=%6").arg(QString("Set_") + tableId).arg((*sensor)->getName()).arg((*sensor)->getX()).arg((*sensor)->getY()).arg((*sensor)->getZ()).arg(sensor-m_sensors->begin() + 1))))
                        QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of a sensor failed."));
                }
                QMessageBox::information(this, tr("Result"), tr("Set of sensors succesfully updated"));
                emit closeDialog();
            }
        }
        else
            QMessageBox::critical(dialogBox, tr("Error"), tr("Selection of the set id in the database failed."));
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Update of the set in the database failed."));
}

void SensorsManagerWidget::deleteSetOfSensors()
{
    QSqlQuery query;

    if(query.exec(QString("SELECT id FROM Sets_of_sensors WHERE name=\"%1\"").arg(m_names->currentText())))
    {
        while(query.next())
        {
            QSqlQuery queryDelete;

            QString tableId = query.value(0).toString();

            if(queryDelete.exec(QString("DROP TABLE %1").arg(QString("Set_") + tableId))) // PLACEHOLDER: arg
            {
                if(queryDelete.exec(QString("DELETE FROM %1 WHERE name=\"%2\"").arg("Sets_of_sensors").arg(m_names->currentText())))
                {
                    QMessageBox::information(this, tr("Result"), tr("Set of sensors succesfully deleted"));
                    emit closeDialog();
                }
                else
                    QMessageBox::critical(dialogBox, tr("Error"), tr("Deletion of the set of sensors in the set list failed."));
            }
            else
                QMessageBox::critical(dialogBox,tr("Error"), tr("Deletion of the set of sensors in the database failed."));
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Selection of the set id in the database failed."));
}

//----------------------------------------------------------------------------
