#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>

#define PORTNAME "COM3"

class QSerialPort;

class Serial : public QObject
{
    Q_OBJECT
public:
    explicit Serial(QObject *parent = 0);
    Serial(int id, QList<qreal> *data, QObject *parent = 0);
    ~Serial();

    void queryStatus(int code = 3);

signals:
    void statusUpdate(bool);
    void closeSerial();

public slots:
    void newSerial();
    void deleteSerial();
    void heartbeat();
    void upload();
    void statusChanged(bool s);

    void handleReadyRead();
    void neuPacket(QByteArray const &d);

private:
    int m_id;
    bool m_status;
    bool m_deleteDone;
    QList<qreal> *m_data;

    QString m_head;
    QString m_tail;

    char *m_heartbeat;

    QSerialPort *m_serial;
};

#endif // SERIAL_H
