#ifndef TIMER_H
#define TIMER_H

#include <QObject>

class Timer : public QObject
{
    Q_OBJECT
public:
    explicit Timer(int interval = 1000, QObject *parent = 0);
    ~Timer();

    int m_time;

signals:
    void timeout();

public slots:
    void onStatusChanged(bool s);
    void timerStart();
    void timerStop();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int m_id;
    int m_interval;
};

#endif // IMER_H
