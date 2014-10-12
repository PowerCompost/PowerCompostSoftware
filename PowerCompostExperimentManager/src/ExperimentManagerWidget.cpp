//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/ExperimentManagerWidget.h"

//----------------------------------------------------------------------------

ExperimentManagerWidget::ExperimentManagerWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database)
{
    QGroupBox *groupBoxExperiment = new QGroupBox(tr("Experiments: "), this);

    m_buttonStart  = new QPushButton(tr("Start experiment"), groupBoxExperiment);
    m_buttonStop   = new QPushButton(tr("Stop experiment"), groupBoxExperiment);
    m_buttonEdit   = new QPushButton(tr("Edit experiment"), groupBoxExperiment);

    QVBoxLayout *layoutGroupBoxExperiment = new QVBoxLayout;
    layoutGroupBoxExperiment->setAlignment(Qt::AlignHCenter);
    layoutGroupBoxExperiment->addWidget(m_buttonStart);
    layoutGroupBoxExperiment->addWidget(m_buttonStop);
    layoutGroupBoxExperiment->addWidget(m_buttonEdit);
    groupBoxExperiment->setLayout(layoutGroupBoxExperiment);

    QHBoxLayout *layoutWidget = new QHBoxLayout;
    layoutWidget->setAlignment(Qt::AlignHCenter);
    layoutWidget->addWidget(groupBoxExperiment);

    setLayout(layoutWidget);

    connect(m_buttonStart, SIGNAL(clicked()), this, SLOT(startExperimentDialog()));
    connect(m_buttonStop, SIGNAL(clicked()), this, SLOT(stopExperimentDialog()));
    connect(m_buttonEdit, SIGNAL(clicked()), this, SLOT(editExperimentDialog()));

    connect(this, SIGNAL(connexionStatusChanged()), this, SLOT(enableWidgets()));

    m_connexionEnabled = false;
    emit connexionStatusChanged();
}

ExperimentManagerWidget::~ExperimentManagerWidget()
{

}

//----------------------------------------------------------------------------

void ExperimentManagerWidget::setConnexionEnabled(bool connexionEnabled)
{
    m_connexionEnabled = connexionEnabled;

    emit connexionStatusChanged();
}

void ExperimentManagerWidget::enableWidgets()
{
    m_buttonStart->setEnabled(m_connexionEnabled);
    m_buttonStop->setEnabled(m_connexionEnabled);
    m_buttonEdit->setEnabled(m_connexionEnabled);
}    

//----------------------------------------------------------------------------

void ExperimentManagerWidget::startExperimentDialog()
{
                m_nameExperiment        = new QLineEdit();
    QLabel      *nameExperimentLabel   = new QLabel(tr("Name of the experiment: "));
    QHBoxLayout *layoutNameExperiment  = new QHBoxLayout;

    layoutNameExperiment->addWidget(nameExperimentLabel);
    layoutNameExperiment->addWidget(m_nameExperiment);

                m_namesTestBenches        = new QComboBox();
    QLabel      *namesTestBenchesLabel    = new QLabel(tr("Test bench: "));
    QHBoxLayout *layoutNamesTestBenches   = new QHBoxLayout;

    layoutNamesTestBenches->addWidget(namesTestBenchesLabel);
    layoutNamesTestBenches->addWidget(m_namesTestBenches);

                m_namesSetsOfSensors        = new QComboBox();
    QLabel      *namesSetsOfSensorsLabel    = new QLabel(tr("Set of sensors: "));
    QHBoxLayout *layoutNamesSetsOfSensors   = new QHBoxLayout;

    layoutNamesSetsOfSensors->addWidget(namesSetsOfSensorsLabel);
    layoutNamesSetsOfSensors->addWidget(m_namesSetsOfSensors);

                m_volumetricMass        = new QLineEdit();
    QLabel      *volumetricMassLabel    = new QLabel(tr("Volumetric mass (kg.m^-3): "));
    QHBoxLayout *layoutVolumetricMass   = new QHBoxLayout;

    layoutVolumetricMass->addWidget(volumetricMassLabel);
    layoutVolumetricMass->addWidget(m_volumetricMass);

                m_thermalCapacity        = new QLineEdit();
    QLabel      *thermalCapacityLabel    = new QLabel(tr("Thermal capacity (J.m^-3): "));
    QHBoxLayout *layoutThermalCapacity   = new QHBoxLayout;

    layoutThermalCapacity->addWidget(thermalCapacityLabel);
    layoutThermalCapacity->addWidget(m_thermalCapacity);

                m_thermalConductivity        = new QLineEdit();
    QLabel      *thermalConductivityLabel    = new QLabel(tr("Thermal conductivity (J.m^-1): "));
    QHBoxLayout *layoutThermalConductivity   = new QHBoxLayout;

    layoutThermalConductivity->addWidget(thermalConductivityLabel);
    layoutThermalConductivity->addWidget(m_thermalConductivity);

                m_comments        = new QTextEdit();
    QLabel      *commentsLabel    = new QLabel(tr("Comments: "));
    QHBoxLayout *layoutComments   = new QHBoxLayout;

    layoutComments->addWidget(commentsLabel);
    layoutComments->addWidget(m_comments);

                m_dateStart        = new QDateEdit();
    QLabel      *dateStartLabel    = new QLabel(tr("Start: "));
    QHBoxLayout *layoutDateStart   = new QHBoxLayout;

    layoutDateStart->addWidget(dateStartLabel);
    layoutDateStart->addWidget(m_dateStart);

                m_buttonDialogStart = new QPushButton(tr("Start"));
    QPushButton *buttonCancel       = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons      = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogStart);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNameExperiment);
    layoutDialog->addLayout(layoutNamesTestBenches);
    layoutDialog->addLayout(layoutNamesSetsOfSensors);
    layoutDialog->addLayout(layoutVolumetricMass);
    layoutDialog->addLayout(layoutThermalCapacity);
    layoutDialog->addLayout(layoutThermalConductivity);
    layoutDialog->addLayout(layoutComments);
    layoutDialog->addLayout(layoutDateStart);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);
    
    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogStart, SIGNAL(clicked()), this, SLOT(startExperiment()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    QSqlQuery query;

    m_namesTestBenches->addItem(tr("Select a test bench..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Test_benches")))
    {
        while(query.next())
        {
            m_namesTestBenches->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_namesSetsOfSensors->addItem(tr("Select a set of sensors..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Sets_of_sensors")))
    {
        while(query.next())
        {
            m_namesSetsOfSensors->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_dateStart->setDateTime(QDateTime::currentDateTime());
    m_dateStart->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    m_buttonDialogStart->setEnabled(false);
    connect(m_nameExperiment, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_namesSetsOfSensors, SIGNAL(currentIndexChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_volumetricMass, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalCapacity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalConductivity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));

    dialogBox->exec();
}

void ExperimentManagerWidget::enableButtonDialogStart()
{
    if(m_nameExperiment->text().isEmpty() || m_namesTestBenches->currentIndex() == 0 || m_namesSetsOfSensors->currentIndex() == 0 || m_volumetricMass->text().isEmpty() || m_thermalCapacity->text().isEmpty() || m_thermalConductivity->text().isEmpty())
        m_buttonDialogStart->setEnabled(false);
    else
        m_buttonDialogStart->setEnabled(true);
}

void ExperimentManagerWidget::stopExperimentDialog()
{
                m_namesExperiments        = new QComboBox();
    QLabel      *namesExperimentsLabel    = new QLabel(tr("Experiments: "));
    QHBoxLayout *layoutNamesExperiments   = new QHBoxLayout;

    layoutNamesExperiments->addWidget(namesExperimentsLabel);
    layoutNamesExperiments->addWidget(m_namesExperiments);

                m_dateStop        = new QDateEdit();
    QLabel      *dateStopLabel    = new QLabel(tr("Stop: "));
    QHBoxLayout *layoutDateStop   = new QHBoxLayout;

    layoutDateStop->addWidget(dateStopLabel);
    layoutDateStop->addWidget(m_dateStop);

                m_buttonDialogStop   = new QPushButton(tr("Stop"));
    QPushButton *buttonCancel        = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons       = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogStop);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNamesExperiments);
    layoutDialog->addLayout(layoutDateStop);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogStop, SIGNAL(clicked()), this, SLOT(stopExperiment()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    QSqlQuery query;

    m_namesExperiments->addItem(tr("Select an experiment..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Experiments")))
    {
        while(query.next())
        {
            m_namesExperiments->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_dateStop->setDateTime(QDateTime::currentDateTime());
    m_dateStop->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    m_buttonDialogStop->setEnabled(false);
    connect(m_namesExperiments, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogStop()));

    dialogBox->exec();
}

void ExperimentManagerWidget::enableButtonDialogStop()
{
    if(m_namesExperiments->currentIndex() == 0)
        m_buttonDialogStop->setEnabled(false);
    else
        m_buttonDialogStop->setEnabled(true);
}

void ExperimentManagerWidget::editExperimentDialog()
{
                m_namesExperiments        = new QComboBox();
    QLabel      *namesExperimentsLabel    = new QLabel(tr("Experiments: "));
    QHBoxLayout *layoutNamesExperiments   = new QHBoxLayout;

    layoutNamesExperiments->addWidget(namesExperimentsLabel);
    layoutNamesExperiments->addWidget(m_namesExperiments);

                m_namesTestBenches        = new QComboBox();
    QLabel      *namesTestBenchesLabel    = new QLabel(tr("Test bench: "));
    QHBoxLayout *layoutNamesTestBenches   = new QHBoxLayout;

    layoutNamesTestBenches->addWidget(namesTestBenchesLabel);
    layoutNamesTestBenches->addWidget(m_namesTestBenches);

                m_namesSetsOfSensors        = new QComboBox();
    QLabel      *namesSetsOfSensorsLabel    = new QLabel(tr("Set of sensors: "));
    QHBoxLayout *layoutNamesSetsOfSensors   = new QHBoxLayout;

    layoutNamesSetsOfSensors->addWidget(namesSetsOfSensorsLabel);
    layoutNamesSetsOfSensors->addWidget(m_namesSetsOfSensors);

                m_volumetricMass        = new QLineEdit();
    QLabel      *volumetricMassLabel    = new QLabel(tr("Volumetric mass (kg.m^-3): "));
    QHBoxLayout *layoutVolumetricMass   = new QHBoxLayout;

    layoutVolumetricMass->addWidget(volumetricMassLabel);
    layoutVolumetricMass->addWidget(m_volumetricMass);

                m_thermalCapacity        = new QLineEdit();
    QLabel      *thermalCapacityLabel    = new QLabel(tr("Thermal capacity (J.m^-3): "));
    QHBoxLayout *layoutThermalCapacity   = new QHBoxLayout;

    layoutThermalCapacity->addWidget(thermalCapacityLabel);
    layoutThermalCapacity->addWidget(m_thermalCapacity);

                m_thermalConductivity        = new QLineEdit();
    QLabel      *thermalConductivityLabel    = new QLabel(tr("Thermal conductivity (J.m^-1): "));
    QHBoxLayout *layoutThermalConductivity   = new QHBoxLayout;

    layoutThermalConductivity->addWidget(thermalConductivityLabel);
    layoutThermalConductivity->addWidget(m_thermalConductivity);

                m_comments        = new QTextEdit();
    QLabel      *commentsLabel    = new QLabel(tr("Comments: "));
    QHBoxLayout *layoutComments   = new QHBoxLayout;

    layoutComments->addWidget(commentsLabel);
    layoutComments->addWidget(m_comments);

                m_dateStart        = new QDateEdit();
    QLabel      *dateStartLabel    = new QLabel(tr("Start: "));
    QHBoxLayout *layoutDateStart   = new QHBoxLayout;

    layoutDateStart->addWidget(dateStartLabel);
    layoutDateStart->addWidget(m_dateStart);

                m_dateStop        = new QDateEdit();
    QLabel      *dateStopLabel    = new QLabel(tr("Stop: "));
    QHBoxLayout *layoutDateStop   = new QHBoxLayout;

    layoutDateStop->addWidget(dateStopLabel);
    layoutDateStop->addWidget(m_dateStop);

                m_buttonDialogEdit = new QPushButton(tr("Edit"));
    QPushButton *buttonCancel      = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons     = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogEdit);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNamesExperiments);
    layoutDialog->addLayout(layoutNamesTestBenches);
    layoutDialog->addLayout(layoutNamesSetsOfSensors);
    layoutDialog->addLayout(layoutVolumetricMass);
    layoutDialog->addLayout(layoutThermalCapacity);
    layoutDialog->addLayout(layoutThermalConductivity);
    layoutDialog->addLayout(layoutComments);
    layoutDialog->addLayout(layoutDateStart);
    layoutDialog->addLayout(layoutDateStop);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogEdit, SIGNAL(clicked()), this, SLOT(editExperiment()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    QSqlQuery query;

    m_namesExperiments->addItem(tr("Select an experiment..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Experiments")))
    {
        while(query.next())
        {
            m_namesExperiments->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_namesTestBenches->addItem(tr("Select a test bench..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Test_benches")))
    {
        while(query.next())
        {
            m_namesTestBenches->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_namesSetsOfSensors->addItem(tr("Select a set of sensors..."));

    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Sets_of_sensors")))
    {
        while(query.next())
        {
            m_namesSetsOfSensors->addItem(query.value(0).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_dateStart->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 
    m_dateStop->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    m_buttonDialogEdit->setEnabled(false);
    connect(m_namesExperiments, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_namesSetsOfSensors, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_volumetricMass, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_thermalCapacity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_thermalConductivity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_namesExperiments, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateExperiment(QString)));

    dialogBox->exec();
}

void ExperimentManagerWidget::enableButtonDialogEdit()
{
    if(m_namesExperiments->currentIndex() == 0 || m_namesTestBenches->currentIndex() == 0 || m_namesSetsOfSensors->currentIndex() == 0 || m_volumetricMass->text().isEmpty() || m_thermalCapacity->text().isEmpty() || m_thermalConductivity->text().isEmpty())
        m_buttonDialogEdit->setEnabled(false);
    else
        m_buttonDialogEdit->setEnabled(true);
}


void ExperimentManagerWidget::updateExperiment(QString experimentName)
{
    QSqlQuery query;
    if(query.exec(QString("SELECT test_bench_id, sensors_set_id, started, stopped, volumetric_mass, thermal_capacity, thermal_conductivity, comments FROM %1 WHERE name=\"%2\"").arg("Experiments").arg(experimentName)))
    {
        while(query.next())
        {
            unsigned short index = 0;

            QSqlQuery queryId;
            
            queryId.exec(QString("SELECT name FROM %1 WHERE id=%2").arg("Test_benches").arg(query.value(index).toString()));
            while(queryId.next())
            {
                if(m_namesTestBenches->findText(queryId.value(0).toString()) >= 0)
                    m_namesTestBenches->setCurrentIndex(m_namesTestBenches->findText(queryId.value(0).toString()));
                else
                    QMessageBox::critical(dialogBox, tr("Error"), tr("Database consistency error: unknown test bench."));
            }

            index++;

            queryId.exec(QString("SELECT name FROM %1 WHERE id=%2").arg("Sets_of_sensors").arg(query.value(index).toString()));
            while(queryId.next())
            {
                if(m_namesSetsOfSensors->findText(queryId.value(0).toString()) >= 0)
                    m_namesSetsOfSensors->setCurrentIndex(m_namesSetsOfSensors->findText(queryId.value(0).toString()));
                else
                    QMessageBox::critical(dialogBox, tr("Error"), tr("Database consistency error: unknown set of sensors."));
            }

            index++;

            m_dateStart->setDateTime(query.value(index++).toDateTime());
            m_dateStop->setDateTime(query.value(index++).toDateTime());
            m_volumetricMass->setText(query.value(index++).toString());
            m_thermalCapacity->setText(query.value(index++).toString());
            m_thermalConductivity->setText(query.value(index++).toString());
            m_comments->setText(query.value(index++).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

//----------------------------------------------------------------------------

void ExperimentManagerWidget::startExperiment()
{
    QSqlQuery query;

    if(query.exec(QString("SELECT * FROM %1 WHERE name = \"%2\"").arg("Experiments").arg(m_nameExperiment->text())))
    {
        if(query.size() > 0)
            QMessageBox::critical(dialogBox, tr("Error"), tr("An experiment with this name is already existing."));
        else
        {
            QString testBenchId;
            if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Test_benches").arg(m_namesTestBenches->currentText())))
            {
                while(query.next())
                {
                    testBenchId = query.value(0).toString();
                }
            }

            QString setOfSensorsId;
            if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Sets_of_sensors").arg(m_namesSetsOfSensors->currentText())))
            {
                while(query.next())
                {
                    setOfSensorsId = query.value(0).toString();
                }
            }

            if(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, \"%5\", NULL, %6, %7, %8, \"%9\")").arg("Experiments").arg(m_nameExperiment->text()).arg(testBenchId).arg(setOfSensorsId).arg(m_dateStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(m_volumetricMass->text()).arg(m_thermalCapacity->text()).arg(m_thermalConductivity->text()).arg(m_comments->toPlainText())))
            {
                QMessageBox::information(this, tr("Result"), tr("Experiment succesfully started"));
                emit closeDialog();
            }
            else
                QMessageBox::information(dialogBox, tr("Info"), QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, \"%5\", NULL, %6, %7, %8, \"%9\")").arg("Experiments").arg(m_nameExperiment->text()).arg(testBenchId).arg(setOfSensorsId).arg(m_dateStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(m_volumetricMass->text()).arg(m_thermalCapacity->text()).arg(m_thermalConductivity->text()).arg(m_comments->toPlainText()));
                //QMessageBox::critical(dialogBox, tr("Error"), tr("Creation of the experiment in the database failed."));
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void ExperimentManagerWidget::stopExperiment()
{
    QSqlQuery query;

    QString experimentId;
    if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Experiments").arg(m_namesExperiments->currentText())))
    {
        while(query.next())
        {
            experimentId = query.value(0).toString();
        }
    }

    if(query.exec(QString("UPDATE %1 SET stopped=\"%2\" WHERE id=%3").arg("Experiments").arg(m_dateStop->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(experimentId)))
    {
        QMessageBox::information(this, tr("Result"), tr("Experiment succesfully stopped"));
        emit closeDialog();
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Stop of the experiment in the database failed."));
}

void ExperimentManagerWidget::editExperiment()
{
    QSqlQuery query;

    QString experimentId;
    if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Experiments").arg(m_namesExperiments->currentText())))
    {
        while(query.next())
        {
            experimentId = query.value(0).toString();
        }
    }

    QString testBenchId;
    if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Test_benches").arg(m_namesTestBenches->currentText())))
    {
        while(query.next())
        {
            testBenchId = query.value(0).toString();
        }
    }

    QString setOfSensorsId;
    if(query.exec(QString("SELECT id FROM %1 WHERE name=\"%2\"").arg("Sets_of_sensors").arg(m_namesSetsOfSensors->currentText())))
    {
        while(query.next())
        {
            setOfSensorsId = query.value(0).toString();
        }
    }

    if(query.exec(QString("UPDATE %1 SET test_bench_id=%2, sensors_set_id=%3, started=\"%4\", stopped=\"%5\", volumetric_mass=%6, thermal_conductivity=%7, thermal_capacity=%8, comments=\"%9\" WHERE id=%10").arg("Experiments").arg(testBenchId).arg(setOfSensorsId).arg(m_dateStart->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(m_dateStop->dateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(m_volumetricMass->text()).arg(m_thermalCapacity->text()).arg(m_thermalConductivity->text()).arg(m_comments->toPlainText()).arg(experimentId)))
    {
        QMessageBox::information(this, tr("Result"), tr("Experiment succesfully edited"));
        emit closeDialog();
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Edition of the experiment in the database failed."));
}

//----------------------------------------------------------------------------
