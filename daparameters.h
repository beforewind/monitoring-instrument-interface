#ifndef DAPARAMETERS_H
#define DAPARAMETERS_H

#include <QObject>
#include <QVariant>

#define DACHANNEL 12

class DAParameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
    Q_PROPERTY(int channel READ channel NOTIFY channelChanged)
public:
    explicit DAParameters(QObject *parent = 0);

    QVariantList parameters();
    void setParameters(QVariantList list);

    int channel();

    int *m_waveType;
    int *m_duty;
    qreal *m_amp;
    int *m_period;

signals:
    void parametersChanged();
    void channelChanged();

public slots:

private:
    QVariantList m_list;
    int m_channel;
};

#endif // DAPARAMETERS_H
