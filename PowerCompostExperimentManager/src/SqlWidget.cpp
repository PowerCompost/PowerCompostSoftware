//----------------------------------------------------------------------------

#include "PowerCompostExperimentManager/SqlWidget.h"

//----------------------------------------------------------------------------

SqlWidget::SqlWidget(QSqlDatabase& database, QWidget *parent)
    : QWidget(parent), m_database(database)
{
    QGroupBox *widgetDBConnexion = new QGroupBox(tr("MySQL server: "), this);

    m_hostName = new QLineEdit("localhost", widgetDBConnexion);
    QLabel *hostNameLabel = new QLabel(tr("Host: "));
    QHBoxLayout *layoutHostName = new QHBoxLayout;
    layoutHostName->setAlignment(Qt::AlignLeft);
    layoutHostName->addWidget(hostNameLabel);
    layoutHostName->addWidget(m_hostName);

    //m_port = new Q(widgetDBConnexion);
    //QLabel *labelPort = new QLabel(tr("Port: "));
    //QHBoxLayout *layoutPort = new QHBoxLayout;
    //layoutPort->setAlignment(Qt::AlignLeft);
    //layoutPort->addWidget(labelPort);
    //layoutPort->addWidget(m_port);

    m_userName = new QLineEdit("root",widgetDBConnexion);
    QLabel *userLabel = new QLabel(tr("User: "));
    QHBoxLayout *layoutUserName = new QHBoxLayout;
    layoutUserName->setAlignment(Qt::AlignLeft);
    layoutUserName->addWidget(userLabel);
    layoutUserName->addWidget(m_userName);

    m_password = new QLineEdit("",widgetDBConnexion);
    m_password->setEchoMode(QLineEdit::Password);
    QLabel *passwordLabel = new QLabel(tr("Password: "));
    QHBoxLayout *layoutPassword = new QHBoxLayout;
    layoutPassword->setAlignment(Qt::AlignLeft);
    layoutPassword->addWidget(passwordLabel);
    layoutPassword->addWidget(m_password);

    m_databaseName = new QLineEdit("PowerCompost", widgetDBConnexion);
    QLabel *databaseLabel = new QLabel(tr("Database: "));
    QHBoxLayout *layoutDatabaseName = new QHBoxLayout;
    layoutDatabaseName->setAlignment(Qt::AlignLeft);
    layoutDatabaseName->addWidget(databaseLabel);
    layoutDatabaseName->addWidget(m_databaseName);

    m_connexionButton = new QPushButton("Connexion", widgetDBConnexion);

    QHBoxLayout *layoutWidgetDBConnexion = new QHBoxLayout;
    layoutWidgetDBConnexion->setAlignment(Qt::AlignHCenter);
    layoutWidgetDBConnexion->addLayout(layoutHostName);
    //layoutWidgetDBConnexion->addLayout(layoutPort);
    layoutWidgetDBConnexion->addLayout(layoutUserName);
    layoutWidgetDBConnexion->addLayout(layoutPassword);
    layoutWidgetDBConnexion->addLayout(layoutDatabaseName);
    layoutWidgetDBConnexion->addWidget(m_connexionButton);
    widgetDBConnexion->setLayout(layoutWidgetDBConnexion);

    QHBoxLayout *layoutWidget = new QHBoxLayout;
    layoutWidget->setAlignment(Qt::AlignHCenter);
    layoutWidget->addWidget(widgetDBConnexion);

    setLayout(layoutWidget);

    connect(m_connexionButton, SIGNAL(clicked()), this, SLOT(changeDBConnexionStatus()));

    m_connexionEnabled = false;

}

SqlWidget::~SqlWidget()
{

}

//----------------------------------------------------------------------------

void SqlWidget::changeDBConnexionStatus()
{
    if(!m_connexionEnabled)
    {
        if(!(m_hostName->text().isEmpty() && m_userName->text().isEmpty() && m_password->text().isEmpty() && m_databaseName->text().isEmpty()))
        {
            m_database = QSqlDatabase::addDatabase("QMYSQL");
            m_database.setHostName(m_hostName->text());
            //m_database->setPort(m_port->text().toInt());
            m_database.setUserName(m_userName->text());
            m_database.setPassword(m_password->text());
            m_database.setDatabaseName(m_databaseName->text());


            if(m_database.open())
            {
                m_connexionEnabled = true;

                m_hostName->setDisabled(true);
                //m_port->setDisabled(true);
                m_userName->setDisabled(true);
                m_password->setDisabled(true);
                m_databaseName->setDisabled(true);
                m_connexionButton->setText(tr("Log off"));
            }
            else
            {
                // PLACEHOLDER: add error handler
                bool placeholder = true;
            }

         }
        // PLACEHOLDER: add error handler
    }
    else
    {
        m_database.close();
        m_connexionEnabled = false;

        m_hostName->setDisabled(false);
        //m_port->setDisabled(false);
        m_userName->setDisabled(false);
        m_password->setDisabled(false);
        m_databaseName->setDisabled(false);
        m_connexionButton->setText(tr("Log on"));
    }
}

//----------------------------------------------------------------------------
