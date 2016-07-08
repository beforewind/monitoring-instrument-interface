TEMPLATE = app

QT += widgets \
    qml \
    quick \
    charts \
    sql \
    webchannel \
    webengine \
    xlsx \
    serialport

SOURCES += \
    main.cpp \
    gasdata.cpp \
    daparameters.cpp \
    datasource.cpp \
    dac.cpp \
    daq.cpp \
    database.cpp \
    device.cpp \
    timer.cpp \
    serial.cpp \
    dio.cpp \
    control.cpp

HEADERS += \
    daparameters.h \
    datasource.h \
    gasdata.h \
    dac.h \
    daq.h \
    database.h \
    device.h \
    usb2089.h \
    timer.h \
    serial.h \
    dio.h \
    control.h

CONFIG += c++14

DISTFILES += \
    qml/main.qml \
    qml/Views/StartView.qml \
    qml/Views/DASettingsView.qml \
    qml/Views/GasView.qml \
    content/html/js/flot/jquery.flot.js \
    content/html/js/flot/jquery.flot.crosshair.js \
    qml/Views/SettingsView.qml \
    qml/Views/DAQChartViews.qml \
    qml/Views/DAOutputView1.qml \
    qml/Views/PipeView.qml

RESOURCES += \
    resources.qrc

win32: LIBS += -L$$PWD/./ -lUSB2089_64
