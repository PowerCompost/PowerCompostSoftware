//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/ExperimentManagerWidget.h"

//----------------------------------------------------------------------------

ExperimentManagerWidget::ExperimentManagerWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database)
{
    connect(this, SIGNAL(connexionStatusChanged()), this, SLOT(enableWidgets()));

    m_connexionEnabled = false;
    emit connexionStatusChanged();
}

ExperimentManagerWidget::~ExperimentManagerWidget()
{

}

//----------------------------------------------------------------------------

void ExperimentManagerWidget::setConnexionEnabled(bool connexionEnabled)
{
    m_connexionEnabled = connexionEnabled;

    emit connexionStatusChanged();
}

void ExperimentManagerWidget::enableWidgets()
{

}    

//----------------------------------------------------------------------------
