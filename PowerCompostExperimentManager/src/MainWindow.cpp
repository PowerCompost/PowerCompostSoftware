//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/MainWindow.h"

//----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PowerCompostExperimentManager");

    m_sqlWidget = new SqlWidget(m_database, this);
   
    m_testBenchManager = new TestBenchManagerWidget(m_database, this);

    m_experimentManager = new ExperimentManagerWidget(m_database, this);

    QHBoxLayout *layoutManagers = new QHBoxLayout;
    layoutManagers->setAlignment(Qt::AlignLeft);
    layoutManagers->addWidget(m_testBenchManager);
    layoutManagers->addWidget(m_experimentManager);

    QWidget* mainWidget = new QWidget(this);    

    QVBoxLayout *layoutMainWidget = new QVBoxLayout;
    layoutMainWidget->setAlignment(Qt::AlignTop);
    layoutMainWidget->addWidget(m_sqlWidget);
    layoutMainWidget->addLayout(layoutManagers);

    mainWidget->setLayout(layoutMainWidget);

    setCentralWidget(mainWidget);

    connect(m_sqlWidget, SIGNAL(connexionEnabled(bool)), m_testBenchManager, SLOT(setConnexionEnabled(bool)));
    connect(m_sqlWidget, SIGNAL(connexionEnabled(bool)), m_experimentManager, SLOT(setConnexionEnabled(bool)));

}

MainWindow::~MainWindow()
{

}

//----------------------------------------------------------------------------
