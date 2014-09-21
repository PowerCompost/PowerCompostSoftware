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

                m_x        = new QLineEdit();
    QLabel      *xLabel    = new QLabel(tr("X (cm): "));
    QHBoxLayout *layoutX   = new QHBoxLayout;

    layoutX->addWidget(xLabel);
    layoutX->addWidget(m_x);

                m_y        = new QLineEdit();
    QLabel      *yLabel    = new QLabel(tr("Y (cm): "));
    QHBoxLayout *layoutY   = new QHBoxLayout;

    layoutY->addWidget(yLabel);
    layoutY->addWidget(m_y);

                m_z        = new QLineEdit();
    QLabel      *zLabel    = new QLabel(tr("Z (cm): "));
    QHBoxLayout *layoutZ   = new QHBoxLayout;

    layoutZ->addWidget(zLabel);
    layoutZ->addWidget(m_z);

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
    layoutDialog->addLayout(layoutX);
    layoutDialog->addLayout(layoutY);
    layoutDialog->addLayout(layoutZ);
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

    m_dateStart->setDateTime(QDateTime::currentDateTime());
    m_dateStart->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    m_buttonDialogStart->setEnabled(false);
    connect(m_nameExperiment, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_x, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_y, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_z, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_volumetricMass, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalCapacity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalConductivity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTestBench(QString)));

    dialogBox->exec();
}

void ExperimentManagerWidget::enableButtonDialogStart()
{
    if(m_nameExperiment->text().isEmpty() || m_namesTestBenches->currentIndex() == 0 || m_x->text().isEmpty() || m_y->text().isEmpty() || m_z->text().isEmpty() || m_volumetricMass->text().isEmpty() || m_thermalCapacity->text().isEmpty() || m_thermalConductivity->text().isEmpty())
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

                m_x        = new QLineEdit();
    QLabel      *xLabel    = new QLabel(tr("X (cm): "));
    QHBoxLayout *layoutX   = new QHBoxLayout;

    layoutX->addWidget(xLabel);
    layoutX->addWidget(m_x);

                m_y        = new QLineEdit();
    QLabel      *yLabel    = new QLabel(tr("Y (cm): "));
    QHBoxLayout *layoutY   = new QHBoxLayout;

    layoutY->addWidget(yLabel);
    layoutY->addWidget(m_y);

                m_z        = new QLineEdit();
    QLabel      *zLabel    = new QLabel(tr("Z (cm): "));
    QHBoxLayout *layoutZ   = new QHBoxLayout;

    layoutZ->addWidget(zLabel);
    layoutZ->addWidget(m_z);

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
    layoutDialog->addLayout(layoutX);
    layoutDialog->addLayout(layoutY);
    layoutDialog->addLayout(layoutZ);
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

    m_dateStart->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    if(query.exec(QString("SELECT %1 FROM %2").arg("initial_time").arg("Experiments")))
    {
        while(query.next())
        {
            m_dateStart->setDateTime(query.value(0).toDateTime());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_dateStop->setDisplayFormat(QString("yyyy-MM-dd hh:mm:ss")); 

    if(query.exec(QString("SELECT %1 FROM %2").arg("final_time").arg("Experiments")))
    {
        while(query.next())
        {
            m_dateStop->setDateTime(query.value(0).toDateTime());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));

    m_buttonDialogEdit->setEnabled(false);
    connect(m_namesExperiments, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(int)), this, SLOT(enableButtonDialogEdit()));
    connect(m_x, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_y, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_z, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_volumetricMass, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalCapacity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_thermalConductivity, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogStart()));
    connect(m_namesExperiments, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateExperiment(QString)));
    connect(m_namesTestBenches, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTestBench(QString)));

    dialogBox->exec();
}

void ExperimentManagerWidget::enableButtonDialogEdit()
{
    if(m_namesExperiments->currentIndex() == 0 || m_namesTestBenches->currentIndex() == 0 || m_x->text().isEmpty() || m_y->text().isEmpty() || m_z->text().isEmpty() || m_volumetricMass->text().isEmpty() || m_thermalCapacity->text().isEmpty() || m_thermalConductivity->text().isEmpty())
        m_buttonDialogEdit->setEnabled(false);
    else
        m_buttonDialogEdit->setEnabled(true);
}

void ExperimentManagerWidget::updateTestBench(QString testBenchName)
{
    QSqlQuery query;
    if(query.exec(QString("SELECT x,y,z,insulation_thickness FROM %1 WHERE name = \"%2\"").arg("Test_benches").arg(testBenchName)))
    {
        while(query.next())
        {
            unsigned short index = 0;
            m_x->setText(query.value(index++).toString());
            m_y->setText(query.value(index++).toString());
            m_z->setText(query.value(index++).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void ExperimentManagerWidget::updateExperiment(QString experimentName)
{
    QSqlQuery query;
    if(query.exec(QString("SELECT test_bench_name,x,y,z,volumetric_mass,thermal_capacity,thermal_conductivity,comments,initial_time,final_time FROM %1 WHERE name = \"%2\"").arg("Experiments").arg(experimentName)))
    {
        while(query.next())
        {
            unsigned short index = 0;

            if(m_namesTestBenches->findText(query.value(index).toString()) >= 0)
                m_namesTestBenches->setCurrentIndex(m_namesTestBenches->findText(query.value(index).toString()));
            else
                QMessageBox::critical(dialogBox, tr("Error"), tr("Database consistency error: unknown test bench."));

            index++;

            m_x->setText(query.value(index++).toString());
            m_y->setText(query.value(index++).toString());
            m_z->setText(query.value(index++).toString());
            m_volumetricMass->setText(query.value(index++).toString());
            m_thermalCapacity->setText(query.value(index++).toString());
            m_thermalConductivity->setText(query.value(index++).toString());
            m_comments->setText(query.value(index++).toString());
            m_dateStart->setDateTime(query.value(index++).toDateTime());
            m_dateStop->setDateTime(query.value(index++).toDateTime());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

//----------------------------------------------------------------------------

void ExperimentManagerWidget::startExperiment()
{
//    QSqlQuery query;
//
//    if(query.exec(QString("SELECT * FROM %1 WHERE name = \"%2\"").arg("Test_benches").arg(m_name->text())))
//    {
//        if(query.size() > 0)
//            QMessageBox::critical(dialogBox, tr("Error"), tr("A test bench with this name is already existing."));
//        else
//        {
//            if(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, %5)").arg("Test_benches").arg(m_name->text()).arg(m_x->text().toInt()).arg(m_y->text().toInt()).arg(m_z->text().toInt())))
//            {
//                QMessageBox::information(this, tr("Result"), tr("Test bench succesfully started"));
//                emit closeDialog();
//            }
//            else
//                QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of the test bench in the database failed."));
//        }
//    }
//    else
//        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void ExperimentManagerWidget::stopExperiment()
{
//    QSqlQuery query;
//
//    if(query.exec(QString("DELETE FROM %1 WHERE name=\"%2\"").arg("Test_benches").arg(m_names->currentText())))
//    {
//        QMessageBox::information(this, tr("Result"), tr("Test bench succesfully stoped"));
//        emit closeDialog();
//    }
//    else
//        QMessageBox::critical(dialogBox, tr("Error"), tr("Edition of the test bench in the database failed."));
}

void ExperimentManagerWidget::editExperiment()
{
//    QSqlQuery query;
//
//    if(query.exec(QString("UPDATE %1 SET x=%2, y=%3, z=%4 WHERE name=\"%5\"").arg("Test_benches").arg(m_x->text().toInt()).arg(m_y->text().toInt()).arg(m_z->text().toInt()).arg(m_names->currentText())))
//    {
//        QMessageBox::information(this, tr("Result"), tr("Test bench succesfully edited"));
//        emit closeDialog();
//    }
//    else
//        QMessageBox::critical(dialogBox, tr("Error"), tr("Deletion of the test bench in the database failed."));
}

//----------------------------------------------------------------------------
