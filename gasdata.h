#ifndef GASDATA_H
#define GASDATA_H

#include <QObject>

class QTimer;

class GasData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal time READ time NOTIFY timeChanged)
    Q_PROPERTY(qreal so2 READ so2 NOTIFY so2Changed)
    Q_PROPERTY(qreal no2 READ no2 NOTIFY no2Changed)
    Q_PROPERTY(qreal o3 READ o3 NOTIFY o3Changed)
    Q_PROPERTY(qreal co READ co NOTIFY coChanged)
    Q_PROPERTY(qreal rh READ rh NOTIFY rhChanged)
    Q_PROPERTY(qreal temp READ temp NOTIFY tempChanged)

public:
    explicit GasData(QObject *parent = 0);
    GasData(qint16 *buffer);

    qreal time();
    qreal so2();
    qreal no2();
    qreal o3();
    qreal co();
    qreal rh();
    qreal temp();

    QList<qreal> m_data;

signals:
    void timeChanged();
    void so2Changed();
    void no2Changed();
    void coChanged();
    void o3Changed();
    void rhChanged();
    void tempChanged();
    void dataUpdated();

public slots:
    void newData();

private:
    QList<qreal> m_legend;
    qint16 *m_buffer;
};

#endif // GASDATA_H
