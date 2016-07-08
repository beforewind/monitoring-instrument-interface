#include "timer.h"

Timer::Timer(int interval, QObject *parent) : m_interval(interval),
    QObject(parent)
{

}

Timer::~Timer()
{
    //timerStop();
}

void Timer::onStatusChanged(bool s)
{
    switch(s){
    case true:
        timerStart();
        break;
    case false:
        timerStop();
        break;
    }
}

void Timer::timerStart()
{
    if(m_id < 0)
        m_id = this->startTimer(m_interval);
}

void Timer::timerStop()
{
    if(m_id >= 0)
        this->killTimer(m_id);
    m_id = -1;
}

void Timer::timerEvent(QTimerEvent *event)
{
    m_time++;
    emit(timeout());
}
