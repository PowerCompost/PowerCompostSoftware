//----------------------------------------------------------------------------

#ifndef SENSORSMANAGERWIDGET_H
#define SENSORSMANAGERWIDGET_H

//----------------------------------------------------------------------------

// Specific header files

#include "SensorWidget.h" 

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
#include <QTextEdit>
#include <QLabel>
#include <QSpinBox>
#include <QVector>

//----------------------------------------------------------------------------

class SensorsManagerWidget : public QWidget
{
    Q_OBJECT

public:
    SensorsManagerWidget(QSqlDatabase& database, QWidget *parent = 0);
    ~SensorsManagerWidget();

private:
    QSqlDatabase m_database;

    bool m_connexionEnabled;

    QPushButton *m_buttonAdd;
    QPushButton *m_buttonEdit;
    QPushButton *m_buttonDelete;

    QDialog     *dialogBox;
    unsigned short m_activeWindow;

    QLineEdit   *m_name;
    QComboBox   *m_names;
    QSpinBox    *m_numberOfSensors;
    QVector<SensorWidget*> *m_sensors;
    QVBoxLayout *m_layoutSensors;
    QTextEdit   *m_comments;

    QPushButton *m_buttonDialogAdd;
    QPushButton *m_buttonDialogEdit;
    QPushButton *m_buttonDialogDelete;

signals:
    void connexionStatusChanged();
    void closeDialog();

public slots:
    void setConnexionEnabled(bool);
    void enableWidgets();
    void displaySensorWidgets(int);

    void addSetOfSensorsDialog();
    void enableButtonDialogAdd();

    void editSetOfSensorsDialog();
    void enableButtonDialogEdit();
    void updateSetOfSensors(QString);

    void deleteSetOfSensorsDialog();
    void enableButtonDialogDelete();

    void addSetOfSensors();
    void editSetOfSensors();
    void deleteSetOfSensors();

};

//----------------------------------------------------------------------------

#endif // SENSORSMANAGERWIDGET_H

//----------------------------------------------------------------------------
