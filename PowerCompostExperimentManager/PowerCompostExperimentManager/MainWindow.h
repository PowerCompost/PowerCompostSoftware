//----------------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//----------------------------------------------------------------------------

// Specific header files
#include "TestBenchManagerWidget.h"
#include "SensorsManagerWidget.h"
#include "ExperimentManagerWidget.h"

// Specific libs
#include <SqlWidget/SqlWidget.h>

//----------------------------------------------------------------------------

// Qt headers
#include <QtGui>
#include <QtWidgets>
#include <QtSql>

//----------------------------------------------------------------------------

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSqlDatabase              m_database;
    SqlWidget*                m_sqlWidget;
    TestBenchManagerWidget*   m_testBenchManager;
    SensorsManagerWidget*     m_sensorsManager;
    ExperimentManagerWidget*  m_experimentManager;

signals:

public slots:

};

//----------------------------------------------------------------------------

#endif // MAINWINDOW_H

//----------------------------------------------------------------------------
