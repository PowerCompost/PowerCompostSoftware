//----------------------------------------------------------------------------

#ifndef EXPERIMENTMANAGERWIDGET_H
#define EXPERIMENTMANAGERWIDGET_H

//----------------------------------------------------------------------------

// Specific header files

//----------------------------------------------------------------------------

// Qt headers

#include <QtSql>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

//----------------------------------------------------------------------------

class ExperimentManagerWidget : public QWidget
{
    Q_OBJECT

public:
    ExperimentManagerWidget(QSqlDatabase& database, QWidget *parent = 0);
    ~ExperimentManagerWidget();

private:
    QSqlDatabase m_database;

    bool m_connexionEnabled;

signals:
    void connexionStatusChanged();

public slots:
    void setConnexionEnabled(bool);
    void enableWidgets();

};

//----------------------------------------------------------------------------

#endif // EXPERIMENTMANAGERWIDGET_H

//----------------------------------------------------------------------------
