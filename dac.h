#ifndef DAC_H
#define DAC_H

#include <QObject>
#include "device.h"

#define RES 25.0

class DAParameters;

class DAC : public QObject
{
    Q_OBJECT
public:
    explicit DAC(HANDLE *h, const DAParameters *para, int channel);
    ~DAC();

signals:
    void timeouted(qint64);

public slots:

private slots:
    void onStatusChanged(bool s);
    void echo(const QDateTime &t);

protected:
    void timerEvent(QTimerEvent *event);

private:
    HANDLE *m_handle;
    int m_id;
    int m_channel;
    int m_dev;
    int m_chan;

    int *m_waveType;
    int *m_duty;
    qreal *m_amp;
    int *m_period;
    int m_phase;

    qint64 m_count;
    int m_daVal[2];
};

#endif // DAC_H
