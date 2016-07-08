#ifndef CONTROL_H
#define CONTROL_H

#include <QObject>

class Control : public QObject
{
    Q_OBJECT
public:
    explicit Control(qint16 *buffer);

signals:

public slots:
    void onStatusChanged(bool s);
    void timerStart();
    void timerStop();

    void setRef(int indx, int v);
    void init();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int m_id;
    int m_interval;

    int *m_ref;

    qint16 *m_buffer;
};

#endif // CONTROL_H
