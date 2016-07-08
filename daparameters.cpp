#include "daparameters.h"

DAParameters::DAParameters(QObject *parent) : QObject(parent),
    m_channel(DACHANNEL)
{
    m_duty = new int[DACHANNEL];
    m_waveType = new int[DACHANNEL];
    m_amp = new qreal[DACHANNEL];
    m_period = new int[DACHANNEL];

    for(int i=0; i<DACHANNEL; ++i){
        QVariantList list;
        for(int j=0; j<4; ++j){
            list << QVariant(0);
        }
        m_list << QVariant(list);
    }
}

int DAParameters::channel()
{
    return m_channel;
}

QVariantList DAParameters::parameters()
{
    return m_list;
}

void DAParameters::setParameters(QVariantList list)
{
    m_list = list;
    for(int i=0; i<DACHANNEL; ++i){
        QVariantList list = m_list[i].toList();
        m_waveType[i] = list[0].toInt();
        m_amp[i] = list[1].toDouble();
        m_period[i] = list[2].toInt();
        m_duty[i] = list[3].toInt();
    }
    emit(parametersChanged());
    /*
    qDebug("DAParameters: settings changed");
    for(int i=0; i<DACHANNEL; ++i){
        qDebug("waveType: %d, amp: %.2f, period: %d, duty: %d", m_waveType[i], m_amp[i], m_period[i], m_duty[i]);
    }
    */
}
