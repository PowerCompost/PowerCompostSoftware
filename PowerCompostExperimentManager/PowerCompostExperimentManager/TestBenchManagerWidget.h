//----------------------------------------------------------------------------

#if defined TESTBENCHMANAGERWIDGET
#define TESTBENCHMANAGERWIDGET_COMMON_DLLSPEC  Q_DECL_EXPORT
#else
#define TESTBENCHMANAGERWIDGET_COMMON_DLLSPEC Q_DECL_IMPORT
#endif

#ifndef TESTBENCHMANAGERWIDGET_H
#define TESTBENCHMANAGERWIDGET_H

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

class TESTBENCHMANAGERWIDGET_COMMON_DLLSPEC TestBenchManagerWidget : public QWidget
{
    Q_OBJECT

public:
    TestBenchManagerWidget(QSqlDatabase& database, QWidget *parent = 0);
    ~TestBenchManagerWidget();

private:
    QSqlDatabase m_database;

    bool m_connexionEnabled;

    QPushButton *m_buttonAdd;
    QPushButton *m_buttonEdit;
    QPushButton *m_buttonDelete;

    QDialog     *dialogBox;

    QLineEdit   *m_name;
    QComboBox   *m_names;
    QLineEdit   *m_x;
    QLineEdit   *m_y;
    QLineEdit   *m_z;


signals:
    void connexionStatusChanged();

public slots:
    void setConnexionEnabled(bool);
    void enableWidgets();

    void addTestBenchDialog();
    void editTestBenchDialog();
    void deleteTestBenchDialog();

    void addTestBench();
    void editTestBench();
    void deleteTestBench();

};

//----------------------------------------------------------------------------

#endif // TESTBENCHMANAGERWIDGET_H

//----------------------------------------------------------------------------
