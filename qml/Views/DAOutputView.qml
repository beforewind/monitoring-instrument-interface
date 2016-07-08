import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtCharts 2.1
import Qt.labs.controls 1.0
import QtQuick.Extras 1.4 as Extras

Item {
    property var source: dasource
    Flickable {
        id: channels
        anchors {
            top: parent.top
            left: parent.left
            right: controlPanel.left
            margins: 5
        }
        height: 30
        contentHeight: rowLayout.height
        contentWidth: rowLayout.width
        clip: true
        flickableDirection: Flickable.HorizontalFlick

        RowLayout {
            id: rowLayout

            Repeater {
                id: repeater
                model: chartView.n

                CheckBox {
                    id: control
                    text: "通道" + (index + 1)
                    checked: true
                    indicator: Rectangle {
                        implicitWidth: 20
                        implicitHeight: 20
                        x: text ? (control.mirrored ? control.width - width - control.rightPadding : control.leftPadding) : control.leftPadding + (control.availableWidth - width) / 2
                        y: control.topPadding + (control.availableHeight - height) / 2

                        color: control.enabled ? (control.pressed ? "#e4e4e4" : "#f6f6f6") : "#353637"
                        border.color: control.enabled ? (control.pressed ? "#26282a" : "#353637") : "transparent"

                        Rectangle {
                            x: (parent.width - width) / 2
                            y: (parent.height - height) / 2
                            width: 16
                            height: 16
                            color: chartView.lineColors[index]
                            visible: control.checkState === Qt.Checked
                        }

                        Rectangle {
                            x: (parent.width - width) / 2
                            y: (parent.height - height) / 2
                            width: 16
                            height: 3
                            color: "#353637"
                            visible: control.checkState === Qt.PartiallyChecked
                        }
                    }
                    Component.onCompleted: {
                        chartView.channelChecked[index] = checked;
                    }
                }
            }
        }
    }

    ChartView {
        id: chartView
        visible: StackView.Active
        anchors {
            top: channels.bottom
            bottom: parent.bottom
            left: parent.left
            right: controlPanel.left
        }
        animationOptions: ChartView.NoAnimation
        legend.visible: false
        property int n: 12
        property var sensorSeries: []
        property var channelChecked: []
        property var lineColors: ["black", "cyan", "darkblue", "DodgerBlue ", "red",
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
            titleText: "Time(s)"
            min: source.axisXmin
            max: source.axisXmax
        }

        ValueAxis {
            id: axisY
            titleText: "Voltage(V)"
            min: (dialYOffset.value - dialY.scaleValue()) / 1000
            max: (dialYOffset.value + dialY.scaleValue()) / 1000
        }

        Component.onCompleted: {
            source.generateData(n, dialX.scaleValue() * 60);
            redraw();
        }

        function redraw() {
            chartView.removeAllSeries();
            for(var i=0; i<n; i++) {
                if(channelChecked[i] === false)
                    continue;
                sensorSeries[i] = chartView.createSeries(ChartView.SeriesTypeLine, "通道" + (i+1), axisX, axisY);
                sensorSeries[i].useOpenGL = true;
                sensorSeries[i].color = lineColors[i];
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
        width: 130
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            margins: 10
        }

        ColumnLayout {
            anchors.fill: parent

            Item {
                Layout.fillHeight: true
            }

            Button {
                text: "全部选择"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                onClicked: {
                    for(var i=0; i<repeater.model; i++)
                        repeater.itemAt(i).checked = true;
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Button {
                text: "全部取消"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                onClicked: {
                    for(var i=0; i<repeater.model; i++)
                        repeater.itemAt(i).checked = false;
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Switch {
                id: saveButton
                text: qsTr("保存数据")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                onCheckedChanged: {
                    database.saveData(checked);
                }
                Component.onCompleted: {
                    database.saveData(checked);
                }
            }

            Switch {
                id: positionButton
                checked: false
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                enabled: !saveButton.checked
                onCheckedChanged: {
                    database.tableSwitch(checked);
                }
                Component.onCompleted: {
                    database.tableSwitch(checked);
                }
            }

            Label {
                text: positionButton.checked ? qsTr("继续上一文件") : qsTr("新建文件")
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            }

            Item {
                Layout.fillHeight: true
            }

            Text {
                text: "X axis scale"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            }
            Extras.Dial {
                id: dialX
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                stepSize: 1
                maximumValue: 7
                value: 4

                function scaleValue() {
                    var val = [1/6, 0.25, 0.5, 1, 4, 10, 30, 60];
                    return val[value];
                }

                onValueChanged: {
                    source.updateColCount(dialX.scaleValue() * 60);
                }

            }
            Text {
                text: indicator(dialX.value)
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                function indicator(value) {
                    var label = [10, 15, 30, 1, 4, 10, 30, 60];
                    var unit = [1, 1, 1, 0, 0, 0, 0, 0]
                    var unitText = ["min", "sec"];
                    return label[value] + unitText[unit[value]];
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Text {
                text: "Y axis scale"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            }
            Extras.Dial {
                id: dialY
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                value: 3
                stepSize: 1
                maximumValue: 4

                function scaleValue() {
                    var val = [250, 500, 1, 2.5, 5];
                    var unit = [1, 1, 1000, 1000, 1000];
                    return val[value] * unit[value];
                }
            }
            Text {
                text: indicator(dialY.value)
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                function indicator(value) {
                    var label = [250, 500, 1, 2.5, 5];
                    var unit = [0, 0, 1, 1, 1];
                    var unitText = ["mv", "V"];
                    return label[value] + unitText[unit[value]];
                }
            }

            Item {
                Layout.fillHeight: true
            }

            Text {
                text: "Y axis offset"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            }
            Extras.Dial {
                id: dialYOffset
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                stepSize: 100
                value: 2500
                maximumValue: 5000
                tickmarksVisible: false
            }
            Text {
                text: dialYOffset.value / 1000 + "V"
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            }

            Item {
                Layout.fillHeight: true
            }
        }
    }

    Timer {
        id: refreshTimer
        interval: sampleStatus.refreshRate // 15 Hz
        running: sampleStatus.refresh
        repeat: true
        onTriggered: {
            for(var i=0; i<chartView.n; i++)
                if(chartView.channelChecked[i]){
                    source.update(i, chartView.sensorSeries[i]);
                }
            while(axisX.phase + dialX.scaleValue() * 60 < source.phase)
                axisX.phase += dialX.scaleValue() * 60;
            if(axisX.phase > 3600)
                axisX.phase = 0;
        }
    }

    function update() {
        var channelChecked = chartView.channelChecked;
        for(var i=0; i<chartView.n; i++)
            channelChecked[i] = repeater.itemAt(i).checked;
        chartView.refresh();
        //refreshTimer.running = dial.value === 0;
    }

    function loadSettings() {
        var channelChecked = chartView.channelChecked;
        for(var i=0; i<chartView.n; i++){
            repeater.itemAt(i).checked = channelChecked[i];
        }
    }

    function echo() {
        console.log("timeout from Timer(c++)");
    }
}
