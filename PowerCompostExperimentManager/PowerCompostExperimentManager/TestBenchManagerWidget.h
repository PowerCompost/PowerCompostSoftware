//----------------------------------------------------------------------------

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

class TestBenchManagerWidget : public QWidget
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

    QPushButton *m_buttonDialogAdd;
    QPushButton *m_buttonDialogEdit;
    QPushButton *m_buttonDialogDelete;

signals:
    void connexionStatusChanged();
    void closeDialog();

public slots:
    void setConnexionEnabled(bool);
    void enableWidgets();

    void addTestBenchDialog();
    void enableButtonDialogAdd();

    void editTestBenchDialog();
    void enableButtonDialogEdit();
    void enableButtonDialogEdit(int);
    void updateXYZ(QString);

    void deleteTestBenchDialog();
    void enableButtonDialogDelete(int);

    void addTestBench();
    void editTestBench();
    void deleteTestBench();

};

//----------------------------------------------------------------------------

#endif // TESTBENCHMANAGERWIDGET_H

//----------------------------------------------------------------------------
