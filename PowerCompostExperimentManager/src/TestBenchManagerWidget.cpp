//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/TestBenchManagerWidget.h"

//----------------------------------------------------------------------------

TestBenchManagerWidget::TestBenchManagerWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database)
{
    QGroupBox *groupBoxTestBench = new QGroupBox(tr("Test benches: "), this);

    m_buttonAdd    = new QPushButton(tr("Add test bench"), groupBoxTestBench);
    m_buttonEdit   = new QPushButton(tr("Edit test bench"), groupBoxTestBench);
    m_buttonDelete = new QPushButton(tr("Delete test bench"), groupBoxTestBench);

    QVBoxLayout *layoutGroupBoxTestBench = new QVBoxLayout;
    layoutGroupBoxTestBench->setAlignment(Qt::AlignHCenter);
    layoutGroupBoxTestBench->addWidget(m_buttonAdd);
    layoutGroupBoxTestBench->addWidget(m_buttonEdit);
    layoutGroupBoxTestBench->addWidget(m_buttonDelete);
    groupBoxTestBench->setLayout(layoutGroupBoxTestBench);

    QHBoxLayout *layoutWidget = new QHBoxLayout;
    layoutWidget->setAlignment(Qt::AlignHCenter);
    layoutWidget->addWidget(groupBoxTestBench);

    setLayout(layoutWidget);

    connect(m_buttonAdd, SIGNAL(clicked()), this, SLOT(addTestBenchDialog()));
    connect(m_buttonEdit, SIGNAL(clicked()), this, SLOT(editTestBenchDialog()));
    connect(m_buttonDelete, SIGNAL(clicked()), this, SLOT(deleteTestBenchDialog()));

    connect(this, SIGNAL(connexionStatusChanged()), this, SLOT(enableWidgets()));

    m_connexionEnabled = false;
    emit connexionStatusChanged();
}

TestBenchManagerWidget::~TestBenchManagerWidget()
{

}

//----------------------------------------------------------------------------

void TestBenchManagerWidget::setConnexionEnabled(bool connexionEnabled)
{
    m_connexionEnabled = connexionEnabled;

    emit connexionStatusChanged();
}

void TestBenchManagerWidget::enableWidgets()
{
    m_buttonAdd->setEnabled(m_connexionEnabled);
    m_buttonEdit->setEnabled(m_connexionEnabled);
    m_buttonDelete->setEnabled(m_connexionEnabled);
}    

//----------------------------------------------------------------------------

void TestBenchManagerWidget::addTestBenchDialog()
{
                m_name        = new QLineEdit();
    QLabel      *nameLabel    = new QLabel(tr("Name: "));
    QHBoxLayout *layoutName   = new QHBoxLayout;

    layoutName->addWidget(nameLabel);
    layoutName->addWidget(m_name);

                m_length        = new QLineEdit();
    QLabel      *lengthLabel    = new QLabel(tr("X (cm): "));
    QHBoxLayout *layoutLength   = new QHBoxLayout;

    layoutLength->addWidget(lengthLabel);
    layoutLength->addWidget(m_length);

                m_width        = new QLineEdit();
    QLabel      *widthLabel    = new QLabel(tr("Y (cm): "));
    QHBoxLayout *layoutWidth   = new QHBoxLayout;

    layoutWidth->addWidget(widthLabel);
    layoutWidth->addWidget(m_width);

                m_height        = new QLineEdit();
    QLabel      *heightLabel    = new QLabel(tr("Z (cm): "));
    QHBoxLayout *layoutHeight   = new QHBoxLayout;

    layoutHeight->addWidget(heightLabel);
    layoutHeight->addWidget(m_height);

                m_insulationThickness        = new QLineEdit();
    QLabel      *insulationThicknessLabel    = new QLabel(tr("Insulation thickness (cm): "));
    QHBoxLayout *layoutInsulationThickness   = new QHBoxLayout;

    layoutInsulationThickness->addWidget(insulationThicknessLabel);
    layoutInsulationThickness->addWidget(m_insulationThickness);

                m_buttonDialogAdd = new QPushButton(tr("Add"));
    QPushButton *buttonCancel     = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons    = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogAdd);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutName);
    layoutDialog->addLayout(layoutLength);
    layoutDialog->addLayout(layoutWidth);
    layoutDialog->addLayout(layoutHeight);
    layoutDialog->addLayout(layoutInsulationThickness);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);
    
    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogAdd, SIGNAL(clicked()), this, SLOT(addTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_buttonDialogAdd->setEnabled(false);
    connect(m_name, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_length, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_width, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_height, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));
    connect(m_insulationThickness, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogAdd()));

    dialogBox->exec();
}

void TestBenchManagerWidget::enableButtonDialogAdd()
{
    if(m_name->text().isEmpty() || m_length->text().isEmpty() || m_width->text().isEmpty() || m_height->text().isEmpty() || m_insulationThickness->text().isEmpty())
        m_buttonDialogAdd->setEnabled(false);
    else
        m_buttonDialogAdd->setEnabled(true);
}

void TestBenchManagerWidget::editTestBenchDialog()
{
                m_names        = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Test benches: "));
    QHBoxLayout *layoutNames   = new QHBoxLayout;

    layoutNames->addWidget(namesLabel);
    layoutNames->addWidget(m_names);

                m_length        = new QLineEdit();
    QLabel      *lengthLabel    = new QLabel(tr("X (cm): "));
    QHBoxLayout *layoutLength   = new QHBoxLayout;

    layoutLength->addWidget(lengthLabel);
    layoutLength->addWidget(m_length);

                m_width        = new QLineEdit();
    QLabel      *widthLabel    = new QLabel(tr("Y (cm): "));
    QHBoxLayout *layoutWidth   = new QHBoxLayout;

    layoutWidth->addWidget(widthLabel);
    layoutWidth->addWidget(m_width);

                m_height        = new QLineEdit();
    QLabel      *heightLabel    = new QLabel(tr("Z (cm): "));
    QHBoxLayout *layoutHeight   = new QHBoxLayout;

    layoutHeight->addWidget(heightLabel);
    layoutHeight->addWidget(m_height);

                m_insulationThickness        = new QLineEdit();
    QLabel      *insulationThicknessLabel    = new QLabel(tr("Insulation thickness (cm): "));
    QHBoxLayout *layoutInsulationThickness   = new QHBoxLayout;

    layoutInsulationThickness->addWidget(insulationThicknessLabel);
    layoutInsulationThickness->addWidget(m_insulationThickness);

                m_buttonDialogEdit = new QPushButton(tr("Edit"));
    QPushButton *buttonCancel      = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons     = new QHBoxLayout;

    layoutButtons->addWidget(m_buttonDialogEdit);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNames);
    layoutDialog->addLayout(layoutLength);
    layoutDialog->addLayout(layoutWidth);
    layoutDialog->addLayout(layoutHeight);
    layoutDialog->addLayout(layoutInsulationThickness);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(this, SIGNAL(closeDialog()), dialogBox, SLOT(accept()));
    connect(m_buttonDialogEdit, SIGNAL(clicked()), this, SLOT(editTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_names->addItem(tr("Select a test bench..."));
    QSqlQuery query;
    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Test_benches")))
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
    connect(m_length, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_width, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_height, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));
    connect(m_insulationThickness, SIGNAL(textChanged(QString)), this, SLOT(enableButtonDialogEdit()));

    connect(m_names, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateTestBench(QString)));

    dialogBox->exec();
}

void TestBenchManagerWidget::enableButtonDialogEdit()
{
    if(m_names->currentIndex() == 0 || m_length->text().isEmpty() || m_width->text().isEmpty() || m_height->text().isEmpty() || m_insulationThickness->text().isEmpty())
        m_buttonDialogEdit->setEnabled(false);
    else
        m_buttonDialogEdit->setEnabled(true);
}

void TestBenchManagerWidget::updateTestBench(QString testBenchName)
{
    QSqlQuery query;
    if(query.exec(QString("SELECT x,y,z,insulation_thickness FROM %1 WHERE name = \"%2\"").arg("Test_benches").arg(testBenchName)))
    {
        while(query.next())
        {
            unsigned short index = 0;
            m_length->setText(query.value(index++).toString());
            m_width->setText(query.value(index++).toString());
            m_height->setText(query.value(index++).toString());
            m_insulationThickness->setText(query.value(index++).toString());
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void TestBenchManagerWidget::deleteTestBenchDialog()
{
                m_names        = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Test benches: "));
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
    connect(m_buttonDialogDelete, SIGNAL(clicked()), this, SLOT(deleteTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    m_names->addItem(tr("Select a test bench..."));
    QSqlQuery query;
    if(query.exec(QString("SELECT %1 FROM %2").arg("name").arg("Test_benches")))
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

void TestBenchManagerWidget::enableButtonDialogDelete()
{
    if(m_names->currentIndex() == 0)
        m_buttonDialogDelete->setEnabled(false);
    else
        m_buttonDialogDelete->setEnabled(true);
}

//----------------------------------------------------------------------------

void TestBenchManagerWidget::addTestBench()
{
    QSqlQuery query;

    if(query.exec(QString("SELECT * FROM %1 WHERE name = \"%2\"").arg("Test_benches").arg(m_name->text())))
    {
        if(query.size() > 0)
            QMessageBox::critical(dialogBox, tr("Error"), tr("A test bench with this name is already existing."));
        else
        {
            if(query.exec(QString("INSERT INTO %1 VALUES(NULL, \"%2\", %3, %4, %5, %6)").arg("Test_benches").arg(m_name->text()).arg(m_length->text().toInt()).arg(m_width->text().toInt()).arg(m_height->text().toInt()).arg(m_insulationThickness->text().toInt())))
            {
                QMessageBox::information(this, tr("Result"), tr("Test bench succesfully added"));
                emit closeDialog();
            }
            else
                QMessageBox::critical(dialogBox, tr("Error"), tr("Insertion of the test bench in the database failed."));
        }
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Unsucessful SELECT query."));
}

void TestBenchManagerWidget::editTestBench()
{
    QSqlQuery query;

    if(query.exec(QString("UPDATE %1 SET length=%2, width=%3, height=%4, insulation_thickness=%5 WHERE name=\"%6\"").arg("Test_benches").arg(m_length->text().toInt()).arg(m_width->text().toInt()).arg(m_height->text().toInt()).arg(m_insulationThickness->text().toInt()).arg(m_names->currentText())))
    {
        QMessageBox::information(this, tr("Result"), tr("Test bench succesfully edited"));
        emit closeDialog();
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Edition of the test bench in the database failed."));
}

void TestBenchManagerWidget::deleteTestBench()
{
    QSqlQuery query;

    if(query.exec(QString("DELETE FROM %1 WHERE name=\"%2\"").arg("Test_benches").arg(m_names->currentText())))
    {
        QMessageBox::information(this, tr("Result"), tr("Test bench succesfully deleted"));
        emit closeDialog();
    }
    else
        QMessageBox::critical(dialogBox, tr("Error"), tr("Deletion of the test bench in the database failed."));

}

//----------------------------------------------------------------------------
