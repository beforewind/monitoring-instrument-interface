#include "control.h"

Control::Control(qint16 *buffer)
{
    m_buffer = buffer;
}

void Control::init()
{
    m_ref = new int[10];
}

void Control::setRef(int indx, int v)
{
    qDebug("Control: refChanged(%d, %d)", indx, v);
    m_ref[indx] = v;
}

void Control::onStatusChanged(bool s)
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

void Control::timerStart()
{
    if(m_id < 0)
        m_id = this->startTimer(1000);
}

void Control::timerStop()
{
    if(m_id >= 0)
        this->killTimer(m_id);
    m_id = -1;
}

void Control::timerEvent(QTimerEvent *event)
{
    qDebug("Control: timerEvent()");
}
