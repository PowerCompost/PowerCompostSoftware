//----------------------------------------------------------------------------

#include "PowerCompostLab/MainWindow.h"

//----------------------------------------------------------------------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PowerCompost");
    
    m_sqlWidget = new SqlWidget(m_database, this);

    QWidget* mainWidget = new QWidget(this);
    
    QVBoxLayout *layoutMainWidget = new QVBoxLayout;
    layoutMainWidget->setAlignment(Qt::AlignTop);
    layoutMainWidget->addWidget(m_sqlWidget);
    mainWidget->setLayout(layoutMainWidget);

    setCentralWidget(mainWidget);
}

MainWindow::~MainWindow()
{

}

//----------------------------------------------------------------------------
