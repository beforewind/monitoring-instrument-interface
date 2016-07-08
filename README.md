# monitoring-instrument-interface
A Qt/QML GUI for PC-based portable ambient air pollution monitoring instrument

# Qt

Version 5.6

- Qt Quick Controls
- Qt WebEngine
- Qt Labs Controls(TP)

- [QuickPlot](http://www.arnorehn.de/blog/2014/12/quickplot-a-collection-of-native-qtquick-plot-items/): A collection of native QtQuick plotting items

- [QtXlsxWriter](https://github.com/VSRonin/QtXlsxWriter): .xlsx file reader and writer for Qt5

- [Qt Charts](https://blog.qt.io/blog/2016/01/18/qt-charts-2-1-0-release/): multichannel oscilloscope for AD/DA

# driver

USB2089
Beijing Art Technology

```
tableyyyy_mm_dd_HH_MM_SS_XXX(
    id bigserial,
    time timestamp,
    temp real,
    rh real,
    advol0~11 real,
    davol0~11 real,
    do0~2 real
);
```
PostgreSQL 9.5

# Documentation

see [paper](https://github.com/hjhee/Bachelorarbeit/raw/master/main.pdf) and [blog](https://hjhee.github.io/2016/06/28/ciq35g3av0001igw4y8cxlhz4/)
