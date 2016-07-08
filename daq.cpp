#include "daq.h"
#include <QDateTime>

DAQ::DAQ(HANDLE *handle, qint16 *buffer) : m_handle(handle),
    m_ADBuffer(buffer),
    m_id(-1),
    m_period(100)
{
    //connect(this, SIGNAL(acquired(QDateTime)), this, SLOT(echo(QDateTime)));
}

void DAQ::onThreadStart()
{
    m_buff = new SHORT[256];
}

DAQ::~DAQ()
{
    if(m_id >= 0)
        this->killTimer(m_id);
    m_id = -1;
    delete m_buff;
}

void DAQ::onStatusChanged(bool s)
{
    switch(s){
    case false:
        if(m_id >= 0)
            this->killTimer(m_id);
        m_id = -1;
        break;
    case true:
        if(m_id < 0)
            m_id = this->startTimer(m_period, Qt::PreciseTimer);
        break;
    }
}

void DAQ::setPeriod(int p)
{
    m_period = p;
    if(m_id >= 0){
        this->killTimer(m_id);
        m_id = this->startTimer(m_period, Qt::PreciseTimer);
    }
}

void DAQ::timerEvent(QTimerEvent *event)
{
    LONG c;
    QDateTime datetime = QDateTime::currentDateTime();
    for(int chan=0; chan<DEVCNT; ++chan){
        USB2089_ReadDeviceAD(m_handle[chan], m_buff+16*chan, 32, &c);
        for(int i=0; i<16; ++i)
            m_ADBuffer[chan*16+i]=m_buff[chan*16+i]&0x3FFF;
    }
    /*
    for(int i=0; i<32; ++i)
        qDebug("m_ADBuffer[%d]=%f", i, m_ADBuffer[i]*5.0/16384.0);
    */
    emit(acquired(datetime));
}

void DAQ::echo(const QDateTime &t)
{
    static QDateTime last_time;
    qDebug("DAQ: time: %lld, value: %f", (t.toMSecsSinceEpoch() - last_time.toMSecsSinceEpoch()), m_ADBuffer[+0] * 5000.0 / 16384.0);
    last_time = t;
}
