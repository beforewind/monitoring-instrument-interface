#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QThread>
#include "usb2089.h"
#include <QDateTime>
#define DEVCNT 2
#define DEVID 0

class Serial;
class DAQ;
class DAC;
class Control;
class DIO;
class DAParameters;
class GasData;
class DataSource;
class Timer;

class Device : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int time READ time NOTIFY timeChanged)
    Q_PROPERTY(bool status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QList<bool> doStatus READ doStatus NOTIFY doStatusChanged)
public:
    explicit Device(qint16 *buffer);
    ~Device();

    Q_INVOKABLE void devStart();
    Q_INVOKABLE void devStop();
    Q_INVOKABLE void setSamplePeriod(int p);
    Q_INVOKABLE void setDO(int indx, bool val);
    Q_INVOKABLE void setControl(int indx, int v);

    bool status();
    QList<bool> doStatus();

    int time();

    DataSource *m_adSource;
    DataSource *m_daSource;
    GasData *m_gasData;
    DAParameters *m_daPara;

    int m_samplePeriod;

signals:
    void statusChanged(bool);
    void timeChanged();
    void doStatusChanged();
    void acquired(const QDateTime&);
    void samplePeriodChanged(int);

    void heartbeatStart();
    void heartbeatStop();
    void serialClose();
    void setRef(int,int);

public slots:
    void setStatus(bool s);

private:
    bool m_status;

    Serial *m_serial;

    HANDLE m_handle[2];

    DIO *m_dio;
    QList<bool> m_DOSts;

    DAQ *m_daq;
    QList<DAC *> m_dac;

    Control *m_ctrl;

    Timer *m_devTimer;
    Timer *m_serialTimer;
    Timer *m_gasTimer;

    QThread *m_threadDAQ;
    QList<QThread *> m_threadDAC;
    QThread *m_threadTimer;
    QThread *m_threadCtrl;
    QThread *m_threadGasTimer;
    QThread *m_threadSerial;
};

#endif // DEVICE_H
