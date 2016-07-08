#include "device.h"
#include "serial.h"
#include "daq.h"
#include "dac.h"
#include "dio.h"
#include "control.h"
#include "gasdata.h"
#include "daparameters.h"
#include "datasource.h"
#include "timer.h"

Device::Device(qint16 *buffer) : m_status(false)
{
    // AD parameters
    USB2089_PARA_AD *ADPara;
    ADPara = new USB2089_PARA_AD;

    ADPara->ADMode = USB2089_ADMODE_SEQUENCE;
    ADPara->ReadSizeWords = 32;
    ADPara->FirstChannel = 0;
    ADPara->LastChannel = 15;
    ADPara->Frequency = 1000;
    ADPara->GroupInterval = 1000;
    ADPara->Gains = USB2089_GAINS_1MULT;
    ADPara->TriggerMode =  USB2089_TRIGMODE_SOFT;
    ADPara->TriggerType = USB2089_TRIGTYPE_EDGE;
    ADPara->TriggerDir = USB2089_TRIGDIR_POSITIVE;
    ADPara->ClockSource =  0;
    ADPara->bClockOutput =  0;

    // create device handle

    for(int i=0; i<DEVCNT; ++i){
        m_handle[i] = USB2089_CreateDevice(i);
        if(m_handle[i] == INVALID_HANDLE_VALUE){
            //qFatal("Status: devInit(%d) failed", i);
        }

        USB2089_InitDeviceAD(m_handle[i], ADPara);
        for(int j=0; j<8; ++j){
            USB2089_WriteDeviceDA(m_handle[i], 0, j); // DA set to zero
            m_DOSts.append(false);
        }

        BYTE devDOInit[8]={0};
        USB2089_SetDeviceDO(m_handle[i], devDOInit); // DO set to false
    }

    m_gasData = new GasData(buffer); // GasView.qml
    m_gasTimer = new Timer; // timer: trigger gasData -> update data
    m_threadGasTimer = new QThread;
    m_gasTimer->moveToThread(m_threadGasTimer);
    connect(m_gasTimer, SIGNAL(timeout()), m_gasData, SLOT(newData()));
    connect(this, SIGNAL(statusChanged(bool)), m_gasTimer, SLOT(onStatusChanged(bool)));
    connect(m_threadGasTimer, SIGNAL(finished()), m_threadGasTimer, SLOT(deleteLater()));
    m_threadGasTimer->start();

    m_serial = new Serial(DEVID, &(m_gasData->m_data)); // upload gasData
    m_threadSerial = new QThread;
    m_serial->moveToThread(m_threadSerial);
    m_serialTimer = new Timer(10000);
    connect(this, SIGNAL(heartbeatStart()), m_serialTimer, SLOT(timerStart()));
    connect(this, SIGNAL(heartbeatStop()), m_serialTimer, SLOT(timerStop()));
    connect(this, SIGNAL(statusChanged(bool)), m_serial, SLOT(statusChanged(bool)));
    //connect(this, SIGNAL(serialClose()), m_serial, SLOT(serialClose()));
    connect(m_serial, SIGNAL(statusUpdate(bool)), this, SLOT(setStatus(bool)));
    connect(m_serialTimer, SIGNAL(timeout()), m_serial, SLOT(heartbeat()));
    connect(m_gasData, SIGNAL(dataUpdated()), m_serial, SLOT(upload()));
    connect(m_threadSerial, SIGNAL(started()), m_serial, SLOT(newSerial()));
    connect(m_threadSerial, SIGNAL(finished()), m_threadSerial, SLOT(deleteLater()));
    m_threadSerial->start();
    emit(heartbeatStart());

    m_daPara = new DAParameters; // DASettingsView.qml

    m_dio = new DIO(m_handle, &m_DOSts);
    connect(m_dio, SIGNAL(doStatusChanged()), this, SIGNAL(doStatusChanged()));

    m_daq = new DAQ(m_handle, buffer);
    m_threadDAQ = new QThread;
    m_daq->moveToThread(m_threadDAQ);
    connect(m_daq, SIGNAL(acquired(const QDateTime&)), this, SIGNAL(acquired(const QDateTime&)));
    //connect(m_daq, SIGNAL(acquired(const QDateTime&)), m_gasData, SLOT(newData(const QDateTime&)));
    connect(this, SIGNAL(samplePeriodChanged(int)), m_daq, SLOT(setPeriod(int)));
    connect(this, SIGNAL(statusChanged(bool)), m_daq, SLOT(onStatusChanged(bool)));
    connect(m_threadDAQ, SIGNAL(started()), m_daq, SLOT(onThreadStart()));
    connect(m_threadDAQ, SIGNAL(finished()), m_threadDAQ, SLOT(deleteLater()));
    m_threadDAQ->start();

    for(int i=0; i<12; ++i){
        DAC *dac = new DAC(m_handle, m_daPara, i);
        QThread *thread = new QThread;
        dac->moveToThread(thread);
        connect(this, SIGNAL(statusChanged(bool)), dac, SLOT(onStatusChanged(bool)));
        connect(thread, SIGNAL(finished()), dac, SLOT(deleteLater()));
        thread->start();

        m_dac << dac;
        m_threadDAC << thread;
    }

    m_ctrl = new Control(buffer);
    m_threadCtrl = new QThread;
    m_ctrl->moveToThread(m_threadCtrl);
    connect(this, SIGNAL(statusChanged(bool)), m_ctrl, SLOT(onStatusChanged(bool)));
    connect(this, SIGNAL(setRef(int,int)), m_ctrl, SLOT(setRef(int,int)));
    connect(m_threadCtrl, SIGNAL(finished()), m_threadCtrl, SLOT(deleteLater()));
    connect(m_threadCtrl, SIGNAL(started()), m_ctrl, SLOT(init()));
    m_threadCtrl->start();

    m_devTimer = new Timer;
    m_threadTimer = new QThread;
    m_devTimer->moveToThread(m_threadTimer);
    connect(this, SIGNAL(statusChanged(bool)), m_devTimer, SLOT(onStatusChanged(bool)));
    connect(m_threadTimer, SIGNAL(finished()), m_threadTimer, SLOT(deleteLater()));
    connect(m_devTimer, SIGNAL(timeout()), this, SIGNAL(timeChanged()));
    m_threadTimer->start();

    m_adSource = new DataSource(buffer, m_devTimer, this);
    connect(m_daq, SIGNAL(acquired(const QDateTime&)), m_adSource, SLOT(appendData()));
    connect(this, SIGNAL(samplePeriodChanged(int)), m_adSource, SLOT(updatePeriod(int)));

    m_daSource = new DataSource(buffer+16, m_devTimer, this);
    connect(m_daq, SIGNAL(acquired(const QDateTime&)), m_daSource, SLOT(appendData()));
    connect(this, SIGNAL(samplePeriodChanged(int)), m_daSource, SLOT(updatePeriod(int)));

    m_samplePeriod = 100;
    m_daq->setPeriod(m_samplePeriod);
    emit(samplePeriodChanged(m_samplePeriod));
}

Device::~Device()
{
    emit(heartbeatStop());
    setStatus(false);
    delete m_gasData;
    delete m_ctrl;
    delete m_daPara;
    delete m_devTimer;
    delete m_gasTimer;
    //emit(serialClose());
    //delete m_serial;
    //delete m_serialTimer;

    //m_threadSerial->quit();
    m_threadDAQ->quit();
    foreach(QThread *item, m_threadDAC){
        item->quit();
    }
    m_threadTimer->quit();
    m_threadGasTimer->quit();
    m_threadCtrl->quit();

    for(int i=0; i<DEVCNT; ++i){
        USB2089_ReleaseDeviceAD(m_handle[i]);
        USB2089_ReleaseDevice(m_handle[i]);
    }

    m_threadDAQ->wait();
    foreach(QThread *item, m_threadDAC){
        item->wait();
    }
    m_threadTimer->wait();
    m_threadGasTimer->wait();
    m_threadCtrl->wait();
    //m_threadSerial->wait();
}

bool Device::status()
{
    return m_status;
}

void Device::setStatus(bool s)
{
    m_status = s;
    emit(statusChanged(s));
}

void Device::devStart()
{
    qDebug("starting sample");
    setStatus(true);
    //m_devTimer->timerStart();
}

void Device::devStop()
{
    qDebug("stopping sample");
    setStatus(false);
    //m_devTimer->timerStop();
}

void Device::setDO(int indx, bool val)
{
    m_dio->setDO(indx, val);
}

void Device::setControl(int indx, int v)
{
    emit(setRef(indx, v));
}

int Device::time()
{
    return m_devTimer->m_time;
}

QList<bool> Device::doStatus()
{
    return m_DOSts;
}

void Device::setSamplePeriod(int p)
{
    qDebug("Device: sample period: %d", p);
    m_samplePeriod = p;
    emit(samplePeriodChanged(p));
}
