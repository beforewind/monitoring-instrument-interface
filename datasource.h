/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <QtCore/QObject>
#include <QtCharts/QAbstractSeries>

QT_BEGIN_NAMESPACE
class Timer;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal axisXmin READ axisXmin WRITE setAxisXmin NOTIFY axisXminChanged)
    Q_PROPERTY(qreal axisXmax READ axisXmax WRITE setAxisXmax NOTIFY axisXmaxChanged)
    Q_PROPERTY(qreal phase READ phase WRITE setPhase NOTIFY phaseChanged)
public:
    explicit DataSource(qint16 *buffer, Timer *timer, QObject *parent);

    qreal axisXmin();
    qreal axisXmax();
    qreal phase();

    void setAxisXmin(qreal x);
    void setAxisXmax(qreal x);
    void setPhase(qreal x);

Q_SIGNALS:
    void axisXminChanged();
    void axisXmaxChanged();
    void phaseChanged();

public slots:
    void generateData(int rowCount, int colCount);
    void appendData();
    void update(int index, QAbstractSeries *series);
    void updateColCount(uint col);
    void updatePeriod(int p);
    void updateAxis();

private:
    QList<QVector<QPointF> > m_data;
    Timer *m_timer;
    qint16 *m_buffer;
    int m_rowCount;
    int m_colCount;
    quint32 m_phase;
    int m_period;

    qreal m_xMin;
    qreal m_xMax;
};

#endif // DATASOURCE_H
