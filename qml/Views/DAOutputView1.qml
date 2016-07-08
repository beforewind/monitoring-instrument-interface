import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtCharts 2.1
import Qt.labs.controls 1.0
import QtQuick.Extras 1.4 as Extras

Item {
    ChartView {
        id: chartView
        visible: StackView.Active
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: controlPanel.left
        }
        animationOptions: ChartView.NoAnimation
        property int n: 12
        property var sensorSeries: []
        property var channelChecked: []
        property var lineColors: ["black", "cyan", "darkblue", "darkcyan", "red",
        "brown", "magenta", "seagreen", "coral", "darkgoldenrod",
        "darkgreen", "lime", "dimgray", "firebrick"]
        property var lineWidths: [1, 1, 2, 2, 1,
        1, 1, 2, 2, 2,
        1, 2, 1, 2]
        property var lineStyles: [Qt.SolidLine, Qt.SolidLine, Qt.SolidLine, Qt.DashLine, Qt.SolidLine,
        Qt.DashDotLine, Qt.DashDotDotLine, Qt.SolidLine, Qt.SolidLine, Qt.DashLine,
        Qt.DashLine, Qt.SolidLine, Qt.SolidLine, Qt.DashDotDotLine]

        ValueAxis {
            id: axisX
            min: 0
            max: 400
        }

        ValueAxis {
            id: axisY
            min: 0
            max: 5
        }

        Component.onCompleted: {
            zuhoerer.generateData(n, 400);
            redraw();
        }

        function redraw() {
            chartView.removeAllSeries();
            for(var i=0; i<n; i++) {
                //axises[i] = Qt.createQmlObject('import QtCharts 2.1; ValueAxis { visible: false; min: -0.5; max: 400.5 }', chartView);
                if(channelChecked[i] === false)
                    continue;
                sensorSeries[i] = chartView.createSeries(ChartView.SeriesTypeLine, "通道" + i, axisX, axisY);
                sensorSeries[i].useOpenGL = true;
                sensorSeries[i].color = lineColors[i];
                //sensorSeries[i].style = lineStyles[i]
                //sensorSeries[i].width = lineWidths[i];
            }
        }

        function refresh() {
            var running = refreshTimer.running;
            refreshTimer.running = false;
            redraw();
            refreshTimer.running = running;
        }
    }

    Item {
        id: controlPanel
        width: 200
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 10
        }

        ColumnLayout {
            anchors.fill: parent

            Flickable {
                Layout.fillHeight: true
                Layout.fillWidth: true
                contentHeight: columnLayout.height
                contentWidth: parent.width
                clip: true
                ScrollBar.vertical: ScrollBar { }

                ColumnLayout {
                    id: columnLayout
                    anchors.left: parent.left
                    anchors.right: parent.right

                    Repeater {
                        id: repeater
                        model: chartView.n

                        CheckBox {
                            text: "通道" + index
                            checked: true
                            onCheckedChanged: {
                                chartView.channelChecked[index] = checked;
                            }
                            Component.onCompleted: {
                                chartView.channelChecked[index] = checked;
                            }
                        }
                    }
                }
            }
        }
    }

    Timer {
        id: refreshTimer
        interval: 1 / 60 * 1000 // 60 Hz
        running: true
        repeat: true
        onTriggered: {
            for(var i=0; i<chartView.n; i++)
                if(chartView.channelChecked[i])
                    zuhoerer.update(i, chartView.sensorSeries[i]);
        }
    }

    function update() {
        var channelChecked = chartView.channelChecked;
        for(var i=0; i<chartView.n; i++)
            channelChecked[i] = repeater.itemAt(i).checked;
        chartView.refresh();
        refreshTimer.running = dial.value === 0;
    }

    function loadSettings() {
        for(var i=0; i<repeater.model; i++){
            repeater.itemAt(i).checked = chartView.channelChecked[i];
        }
    }
}

