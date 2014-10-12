//----------------------------------------------------------------------------

#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

//----------------------------------------------------------------------------

// Specific header files

//----------------------------------------------------------------------------

// Qt headers

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>

//----------------------------------------------------------------------------

class SensorWidget : public QWidget
{
    Q_OBJECT

public:
    SensorWidget(QWidget *parent = 0);
    ~SensorWidget();

    QString getName();
    int getX();
    int getY();
    int getZ();

    void setName(QString);
    void setX(int);
    void setY(int);
    void setZ(int);

    bool isEmpty();

private:
    QLineEdit   *m_name;
    QLineEdit   *m_x;
    QLineEdit   *m_y;
    QLineEdit   *m_z;

signals:
    void textChanged();

private slots:
    void informTextChanged();

};

//----------------------------------------------------------------------------

#endif // SENSORWIDGET_H

//----------------------------------------------------------------------------
