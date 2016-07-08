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

#include "datasource.h"
#include "device.h"
#include <QtCharts/QXYSeries>
#include <QtCharts/QAreaSeries>
#include <QtQuick/QQuickView>
#include <QtQuick/QQuickItem>
#include <QtCore/QDebug>
#include <QtCore/QtMath>
#include <QtGlobal>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DataSource::DataSource(qint16 *buffer, Timer *timer, QObject *parent) :
    QObject(parent),
    m_timer(timer),
    m_buffer(buffer),
    m_xMin(0.0),
    m_xMax(240),
    m_rowCount(12),
    m_colCount(240),
    m_phase(0)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();
    generateData(m_rowCount, m_colCount);
}

void DataSource::update(int index, QAbstractSeries *series)
{
    if (series) {
        QXYSeries *xySeries = static_cast<QXYSeries *>(series);

        QVector<QPointF> points = m_data.at(index);
        // Use replace instead of clear + append, it's optimized for performance
        xySeries->replace(points);
    }
}

void DataSource::updateAxis()
{
    if(m_phase+m_period >= m_colCount*1000){
        setAxisXmin(m_xMin+m_colCount);
        setAxisXmax(m_xMin+m_colCount);
        setPhase(0);
        for(int i=0; i<m_rowCount; ++i)
            m_data[i].clear();
    }else
        setPhase(m_phase+m_period);
    emit(phaseChanged());
}

void DataSource::appendData()
{
    for(int i=0; i<m_rowCount; ++i){
        m_data[i].append(QPointF((m_xMin*1000+m_phase)/1000.0, m_buffer[i] * 5.0 / 16384.0));
        //points.append(QPointF(m_timer->m_time, m_buffer[i*8+j] * 5000.0 / 16384.0));
    }
    updateAxis();
}

void DataSource::updateColCount(uint col)
{
    if(col*1000>m_phase){
        setAxisXmax(m_xMin+col);
    }else{
        int step = m_phase / (col*1000);
        if(!m_data.at(0).isEmpty()){
            for(int i=0; i<m_rowCount; ++i){
                int a = m_data.at(i).count();
                int b = step*col;
                int min = std::min(a, b);
                m_data[i].remove(0, min);
            }
        }
        setPhase(m_phase % (col*1000));
        setAxisXmin(m_xMin + step*col);
        setAxisXmax(m_xMin + col);
    }
    m_colCount=col;
    updateAxis();
}

void DataSource::updatePeriod(int p)
{
    m_period = p;
}

void DataSource::generateData(int rowCount, int colCount)
{
    // Remove previous data
    foreach (QVector<QPointF> row, m_data)
        row.clear();
    m_data.clear();

    // Append the new data depending on the type
    for (int i(0); i < rowCount; i++) {
        QVector<QPointF> points;
        points.reserve(colCount);
        for (int j(0); j < colCount; j++) {
            qreal x(0);
            qreal y(0);
            points.append(QPointF(x, y));
        }
        m_data.append(points);
    }

    m_rowCount = rowCount;
    m_colCount = colCount;
}

qreal DataSource::axisXmin()
{
    return m_xMin;
}

qreal DataSource::axisXmax()
{
    return m_xMax;
}

qreal DataSource::phase()
{
    return m_phase;
}

void DataSource::setAxisXmax(qreal x)
{
    m_xMax = x;
    emit(axisXmaxChanged());
}

void DataSource::setAxisXmin(qreal x)
{
    m_xMin = x;
    emit(axisXminChanged());
}

void DataSource::setPhase(qreal x)
{
    m_phase = x;
    emit(phaseChanged());
}
