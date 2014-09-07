//----------------------------------------------------------------------------

#if defined SQLWIDGET
#define SQLWIDGET_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define SQLWIDGET_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

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

class SQLWIDGET_COMMON_DLLSPEC SqlWidget : public QWidget
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
    void connexionEnabled(bool);
    void disableWidgets();

public slots:
    void changeConnexionStatus();
    void setDisabledWidgets();
};

//----------------------------------------------------------------------------

#endif // SQLWIDGET_H

//----------------------------------------------------------------------------
