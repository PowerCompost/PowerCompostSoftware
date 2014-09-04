//----------------------------------------------------------------------------

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//----------------------------------------------------------------------------

// Specific header files
#include "PowerCompostLab/SqlWidget.h"

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
    QSqlDatabase m_database;
    SqlWidget*   m_sqlWidget;

signals:

public slots:

};

//----------------------------------------------------------------------------

#endif // MAINWINDOW_H

//----------------------------------------------------------------------------
