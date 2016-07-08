#ifndef DIO_H
#define DIO_H

#include <QObject>
#include "device.h"

class DIO : public QObject
{
    Q_OBJECT
public:
    explicit DIO(HANDLE *handle, QList<bool> *DOSts, QObject *parent = 0);

signals:
    void doStatusChanged();

public slots:
    void setDO(int indx, bool val);
    void setHeat(int indx, int s);

private:
    void setDeviceDO(int n);

    HANDLE *m_handle;
    QList<bool> *m_DOSts;
};

#endif // DIO_H
