#include "dac.h"
#include "daparameters.h"
#include <QtMath>

DAC::DAC(HANDLE *h, const DAParameters *para, int channel) : m_handle(h),
    m_id(-1),
    m_channel(channel),
    m_count(0),
    m_phase(0)
{
    m_duty = para->m_duty + channel;
    m_waveType = para->m_waveType + channel;
    m_amp = para->m_amp + channel;
    m_period = para->m_period + channel;

    m_dev = channel / 8;
    m_chan = channel % 8;

    //connect(this, SIGNAL(timeouted(QDateTime)), this, SLOT(echo(QDateTime)));
}

DAC::~DAC()
{
    if(m_id >= 0)
        this->killTimer(m_id);
    m_id = -1;
    //qDebug("DAC[%d]: count: %lld", m_channel, m_count);
}

void DAC::onStatusChanged(bool s)
{
    //qDebug("m_duty: %d, m_waveType: %d, m_amp: %f, m_period: %d", *m_duty, *m_waveType, *m_amp, *m_period);
    switch(s){
    case false:
        if(m_id >= 0)
            this->killTimer(m_id);
        m_id = -1;
        break;
    case true:
        if(m_id < 0)
            m_id = this->startTimer(20); //, Qt::PreciseTimer);
        break;
    }
}

void DAC::timerEvent(QTimerEvent *event)
{
    qreal x;
    m_daVal[1] = m_daVal[0];

    switch(*m_waveType){
    case 0x00: // square wave
        if(m_phase*100 < (*m_period)/RES*(*m_duty))
            x = (*m_amp)*100;
        else
            x= 0;
        break;
    case 0x01: // constant
        x = (*m_amp)*100;
        break;
    case 0x02: // sinusoid
        x = (*m_amp)*100 * (1+qSin(2*M_PI*m_phase*RES/((*m_period))))*0.5;
        break;
    }

    int t = x / (5000.0/4096);
    m_daVal[0] = (t>4095) ? 4095 : t;
    if(m_daVal[0] != m_daVal[1])
        USB2089_WriteDeviceDA(m_handle[m_dev], m_daVal[0], m_chan);
    m_phase = (RES*(m_phase + 1) >= (*m_period)) ? 0 : m_phase + 1;

    //static int cnt;
    //if(++cnt==10){
    //    qint64 time = QDateTime::currentMSecsSinceEpoch();
    //    emit(timeouted(time));
    //    cnt=0;
    //}
}

void DAC::echo(const QDateTime &t)
{
    static QDateTime last_time;
    qDebug("DAC[%d]: time: %lld, count: %lld", m_channel, (t.toMSecsSinceEpoch() - last_time.toMSecsSinceEpoch()), m_count++);
    last_time = t;
}
