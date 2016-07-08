#include "dio.h"
#include "usb2089.h"

DIO::DIO(HANDLE *handle, QList<bool> *DOSts, QObject *parent) : QObject(parent)
{
    m_handle = handle;
    m_DOSts = DOSts;
}

void DIO::setDO(int indx, bool val)
{
    qDebug("DO[%d]: %d", indx, val);
    if(m_DOSts->at(indx) == val)
        return;
    (*m_DOSts)[indx] = val;

    BYTE devDO[8];

    if(indx < 8){
        setDeviceDO(0);
    }else{
        setDeviceDO(1);
    }
    emit(doStatusChanged());
}

void DIO::setDeviceDO(int n)
{
    BYTE devDO[8];
    for(int i=0; i<8; ++i)
        devDO[i] = (*m_DOSts)[i+n*8];
    USB2089_SetDeviceDO(m_handle[n], devDO);
}

void DIO::setHeat(int indx, int s)
{
    bool a,b;
    switch(s){
    case 0: // do nothing
        a = 0;
        b = 0;
        break;
    case 1: // heat
        a = 0;
        b = 1;
        break;
    case -1: // cool
        a = 1;
        b = 1;
        break;
    }

    qDebug("DIO::setHeat(%d, %d): %X\n", indx, s, a<<1|b);

    if(indx == 0){
        (*m_DOSts)[8] = a;
        (*m_DOSts)[9] = b;
        setDeviceDO(1);
    }else if(indx == 1){
        (*m_DOSts)[10] = a;
        (*m_DOSts)[11] = b;
        setDeviceDO(1);
    }
}
