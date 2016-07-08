#include "gasdata.h"
#include <QDateTime>

GasData::GasData(QObject *parent) : QObject(parent)
{
    qsrand(1);
    for(int i=0; i<5; ++i){
        m_data << 0;
        m_legend << 0;
    }
}

GasData::GasData(qint16 *buffer) : m_buffer(buffer)
{
    for(int i=0; i<7; ++i){
        m_data << 0;
        m_legend << 0;
    }
}

qreal GasData::time()
{
    return m_data[0];
}

qreal GasData::so2()
{
    return m_data[1];
}

qreal GasData::no2()
{
    return m_data[2];
}

qreal GasData::o3()
{
    return m_data[3];
}

qreal GasData::co()
{
    return m_data[4];
}

qreal GasData::rh()
{
    return m_data[6];
}

qreal GasData::temp()
{
    return m_data[5];
}

void GasData::newData()
{
    //m_data[0] += 1;
    //m_data[0] = t.toMSecsSinceEpoch();
    m_data[0] += 1;
    for(int i=1; i<5; ++i){
        qreal y = m_data[i] + (qrand() * 1.0 / RAND_MAX - 0.5) * 50;
        if(y < 0)
            y = 0;
        else if(y > 100)
            y = 100;
        m_data[i] = y;
    }
    m_data[5] = (m_buffer[28] * 5.0 / 16384.0) / 1.0 * 100;
    m_data[6] = (m_buffer[29] * 5.0 / 16384.0) / 1.0 * 100;
    emit(timeChanged());
    emit(so2Changed());
    emit(no2Changed());
    emit(o3Changed());
    emit(coChanged());
    emit(rhChanged());
    emit(tempChanged());
    emit(dataUpdated());
    //qDebug("gasdata: new data comming, m_data[0] = %X", k);
}
