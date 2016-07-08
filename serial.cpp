#include "serial.h"
#include <QSerialPort>
#include <QDataStream>

Serial::Serial(int id, QList<qreal> *data, QObject *parent) : QObject(parent)
{
    m_id = id;
    m_data = data;

    m_head = QString("hjhee");
    std::reverse_copy(m_head.constBegin(), m_head.constEnd(), m_tail.begin());

    connect(this, SIGNAL(closeSerial()), SLOT(deleteSerial()));
}

Serial::Serial(QObject *parent) : QObject(parent)
{
    m_heartbeat = new char[512];
}

Serial::~Serial()
{
    //delete m_serial;
    m_deleteDone = false;
    emit(closeSerial());
    while(!m_deleteDone);
    delete m_heartbeat;
}

void Serial::deleteSerial()
{
    m_serial->flush();
    while(m_serial->bytesToWrite() > 0);
    m_serial->close();
    delete m_serial;
    m_deleteDone = true;
}

void Serial::newSerial()
{
    m_serial = new QSerialPort(PORTNAME);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->open(QIODevice::ReadWrite);

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));
}

void Serial::heartbeat()
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << m_head.toLatin1();
    stream << quint8(0);
    stream << quint8(m_id);
    //stream << m_tail;

    m_serial->write(array);
    //qDebug("serial: heartbeat: id: %d", m_id);
}

static QString toDebug(const QByteArray & line)
{

    QString s;
    uchar c;

    s.append('[');
    for(int i=0; i<line.size(); i++){
        c = line[i];
        s.append(QString("%1 ").arg(c, 2, 10, QChar('0')));
    }
    s.append(']');
    return s;

    /*
    for ( int i=0 ; i < line.size() ; i++ ){
        c = line[i];
        if ( c >= 0x20 && c <= 126 ) {
            s.append(c);
        } else {
            s.append(QString("<%1>").arg(c, 2, 16, QChar('0')));
        }
    }
    return s;
    */
}

void Serial::upload()
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << m_head.toLatin1();
    stream << quint8(1);
    stream << quint8(m_id);
    stream << *m_data;
    /*
    qDebug("data->size(): %d", m_data->size());
    for(int i=0; i<m_data->size(); ++i)
        stream << double(m_data->at(i));
    */

    m_serial->write(array);
    //stream << m_tail;
    //qDebug("serial: upload(%d)", n);
    //qDebug(toDebug(array).toLatin1());
}

void Serial::statusChanged(bool s)
{
    m_status = s;
    queryStatus(2);
}

void Serial::queryStatus(int code)
{
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    stream << m_head.toLatin1();
    stream << quint8(code);
    stream << quint8(m_id);
    stream << quint8(m_status);

    m_serial->write(array);
}

void Serial::handleReadyRead()
{
    QByteArray data = m_serial->readAll();
    while(m_serial->waitForReadyRead(10)){
        data += m_serial->readAll();
    }
    neuPacket(data);
}

void Serial::neuPacket(QByteArray const &d)
{
    qDebug("new packet from server: %s", toDebug(d).toLatin1());
    switch(quint8(d[0])){
    case 0x1: // status changed
        qDebug("status changed: %d", bool(d[1]));
        emit(statusUpdate(bool(d[1])));
        break;
    case 0x3: // query status
        queryStatus();
        break;
    default: // do nothing
        break;
    }
}
