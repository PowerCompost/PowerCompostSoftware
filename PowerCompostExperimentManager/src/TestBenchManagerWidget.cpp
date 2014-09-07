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

    QPushButton *buttonAdd     = new QPushButton(tr("Add"));
    QPushButton *buttonCancel  = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons = new QHBoxLayout;

    layoutButtons->addWidget(buttonAdd);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutName);
    layoutDialog->addLayout(layoutX);
    layoutDialog->addLayout(layoutY);
    layoutDialog->addLayout(layoutZ);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(buttonAdd, SIGNAL(clicked()), this, SLOT(addTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    dialogBox->exec();
}

void TestBenchManagerWidget::editTestBenchDialog()
{
    m_names                    = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Test benches: "));
    QHBoxLayout *layoutNames   = new QHBoxLayout;

    layoutNames->addWidget(namesLabel);
    layoutNames->addWidget(m_names);

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

    QPushButton *buttonEdit    = new QPushButton(tr("Edit"));
    QPushButton *buttonCancel  = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons = new QHBoxLayout;

    layoutButtons->addWidget(buttonEdit);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNames);
    layoutDialog->addLayout(layoutX);
    layoutDialog->addLayout(layoutY);
    layoutDialog->addLayout(layoutZ);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(buttonEdit, SIGNAL(clicked()), this, SLOT(editTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    dialogBox->exec();
}

void TestBenchManagerWidget::deleteTestBenchDialog()
{
    m_names                    = new QComboBox();
    QLabel      *namesLabel    = new QLabel(tr("Test benches: "));
    QHBoxLayout *layoutNames   = new QHBoxLayout;

    layoutNames->addWidget(namesLabel);
    layoutNames->addWidget(m_names);

    QPushButton *buttonDelete  = new QPushButton(tr("Delete"));
    QPushButton *buttonCancel  = new QPushButton(tr("Cancel"));
    QHBoxLayout *layoutButtons = new QHBoxLayout;

    layoutButtons->addWidget(buttonDelete);
    layoutButtons->addWidget(buttonCancel);
   
    QVBoxLayout *layoutDialog = new QVBoxLayout;
    layoutDialog->addLayout(layoutNames);
    layoutDialog->addLayout(layoutButtons);

    dialogBox = new QDialog();
    dialogBox->setLayout(layoutDialog);

    connect(buttonDelete, SIGNAL(clicked()), this, SLOT(deleteTestBench()));
    connect(buttonCancel, SIGNAL(clicked()), dialogBox, SLOT(accept()));

    dialogBox->exec();
}

//----------------------------------------------------------------------------

void TestBenchManagerWidget::addTestBench()
{

}

void TestBenchManagerWidget::editTestBench()
{

}

void TestBenchManagerWidget::deleteTestBench()
{

}

//----------------------------------------------------------------------------
