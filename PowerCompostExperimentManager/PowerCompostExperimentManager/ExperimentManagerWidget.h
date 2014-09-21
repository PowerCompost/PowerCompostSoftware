//----------------------------------------------------------------------------

#ifndef EXPERIMENTMANAGERWIDGET_H
#define EXPERIMENTMANAGERWIDGET_H

//----------------------------------------------------------------------------

// Specific header files

//----------------------------------------------------------------------------

// Qt headers

#include <QtSql>
#include <QtWidgets>

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

    QPushButton *m_buttonStart;
    QPushButton *m_buttonStop;
    QPushButton *m_buttonEdit;

    QDialog     *dialogBox;

    QLineEdit   *m_nameExperiment;
    QComboBox   *m_namesExperiments;
    QComboBox   *m_namesTestBenches;
    QComboBox   *m_namesSetsOfSensors;
    QDateTimeEdit *m_dateStart;
    QDateTimeEdit *m_dateStop;
    QLineEdit *m_volumetricMass;
    QLineEdit *m_thermalCapacity;
    QLineEdit *m_thermalConductivity;
    QTextEdit *m_comments;

    QPushButton *m_buttonDialogStart;
    QPushButton *m_buttonDialogStop;
    QPushButton *m_buttonDialogEdit;

signals:
    void connexionStatusChanged();
    void closeDialog();
    void closeSensorDialog();

public slots:
    void setConnexionEnabled(bool);
    void enableWidgets();

    void startExperimentDialog();
    void enableButtonDialogStart();

    void stopExperimentDialog();
    void enableButtonDialogStop();

    void editExperimentDialog();
    void enableButtonDialogEdit();

    void updateExperiment(QString);

    void startExperiment();
    void stopExperiment();
    void editExperiment();

};

//----------------------------------------------------------------------------

#endif // EXPERIMENTMANAGERWIDGET_H

//----------------------------------------------------------------------------
