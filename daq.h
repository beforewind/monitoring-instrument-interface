#ifndef DAQ_H
#define DAQ_H

#include <QObject>
#include "device.h"

class DAQ : public QObject
{
    Q_OBJECT
public:
    explicit DAQ(HANDLE *handle, qint16 *buffer);
    ~DAQ();

signals:
    void acquired(const QDateTime&);

public slots:
    void setPeriod(int p);
    void onThreadStart();

private slots:
    void onStatusChanged(bool s);
    void echo(const QDateTime &t);

protected:
    void timerEvent(QTimerEvent *event);

private:
    qint16 *m_ADBuffer;
    HANDLE *m_handle;
    SHORT *m_buff;
    int m_id;
    int m_period;
};

#endif // DAQ_H
