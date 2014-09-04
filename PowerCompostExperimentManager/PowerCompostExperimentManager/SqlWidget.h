//----------------------------------------------------------------------------

#ifndef SQLWIDGET_H
#define SQLWIDGET_H

//----------------------------------------------------------------------------

// Specific header files

//----------------------------------------------------------------------------

// Qt headers

#include <QtGui>
#include <QtWidgets>
#include <QtSql>

//----------------------------------------------------------------------------

class SqlWidget : public QWidget
{
    Q_OBJECT

public:
    SqlWidget(QSqlDatabase& database, QWidget *parent = 0);
    ~SqlWidget();

private:
    QSqlDatabase m_database;
    bool m_connexionEnabled;

    QLineEdit   *m_hostName;
    QLineEdit   *m_userName;
    QLineEdit   *m_password;
    //QLineEdit   *m_port;
    QLineEdit   *m_databaseName;
    QPushButton *m_connexionButton;

signals:

public slots:
    void changeDBConnexionStatus();

};

//----------------------------------------------------------------------------

#endif // SQLWIDGET_H

//----------------------------------------------------------------------------
