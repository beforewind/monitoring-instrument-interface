import QtQuick 2.5
import QtQuick.Layouts 1.1
import QtCharts 2.0
import Qt.labs.controls 1.0
import QuickPlot 1.0

Item {
    id: root
    property var settings: []

    Flickable {
        anchors.fill: parent
        contentWidth: parent.width
        contentHeight: content.height
        ScrollBar.vertical: ScrollBar { }
        visible: StackView.Active
        clip: true

        GridLayout {
            id: content
            anchors {
                left: parent.left
                right: parent.right

                margins: 5
            }
            columnSpacing: 5
            columns: 2

            Repeater {
                model: content.columns

                Item {
                    height: 10
                }
            }

            Repeater {
                id: repeater
                model: daParameters.channel

                GroupBox {
                    id:  group
                    title: "通道" + (index + 1)

                    anchors.margins: 5
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    property alias waveType: waveType.currentIndex
                    property alias amp: amp.value
                    property alias period: period.value
                    property alias duty: duty.value

                    onWaveTypeChanged: series.updateChart()
                    onAmpChanged: series.updateChart()
                    onPeriodChanged: series.updateChart()
                    onDutyChanged: series.updateChart()

                    RowLayout {
                        ColumnLayout {
                            id: leftRow

                            GridLayout {
                                columns: 4
                                Label { text: "输入波形设置" }
                                Item { Layout.fillWidth: true }
                                ComboBox {
                                    id: waveType
                                    currentIndex: 1
                                    model: ListModel {
                                        ListElement { text: "方波" }
                                        ListElement { text: "常值" }
                                        ListElement { text: "正弦波" }
                                    }
                                }
                                Item { Layout.fillWidth: true }

                                Label { text: "幅值" }
                                Item { Layout.fillWidth: true }
                                SpinBox {
                                    id: amp
                                    value: 0
                                    from: 0
                                    to: 40
                                    textFromValue: function(value) {
                                        return value/10.0;
                                    }
                                    valueFromText: function(text) {
                                        return Number(text)*10.0;
                                    }
                                }
                                Label { text: "V" }

                                Label { text: "周期" }
                                Item { Layout.fillWidth: true }
                                SpinBox {
                                    id: period
                                    value: 1000
                                    from: 10
                                    to: 3000
                                    stepSize: 10
                                    enabled: waveType.currentIndex != 1
                                    textFromValue: function(value) {
                                        return value;
                                    }
                                    valueFromText: function(text) {
                                        return Number(text);
                                    }
                                }
                                Label { text: "ms" }

                                Label { text: "占空比" }
                                Item { Layout.fillWidth: true }
                                SpinBox {
                                    id: duty
                                    value: 50
                                    to: 100
                                    stepSize: 10
                                    enabled: waveType.currentIndex === 0
                                }
                                Label { text: "%" }
                            }
                        }

                        Rectangle {
                            width: leftRow.width - 30
                            height: leftRow.height
                            border.width: 1
                            border.color: "grey"

                            PlotArea {
                                id: plotArea
                                anchors.fill: parent
                                anchors.margins: 5
                                axes.visible: false
                                hasXTicks: false
                                hasYTicks: false
                                yScaleEngine: FixedScaleEngine {
                                    max: 5.1
                                    min: -0.1
                                }

                                items: [
                                    ScrollingCurve {
                                        id: series
                                        color: "blue"
                                        numPoints: 150

                                        Component.onCompleted: {
                                            updateChart();
                                        }

                                        function updateChart() {
                                            var amp=group.amp/10;
                                            var N=3000/group.period;
                                            var step=numPoints/N;
                                            var pos=step*group.duty/100;
                                            var count=0;
                                            var num=numPoints;
                                            switch(group.waveType) {
                                            case 0x0: { // square wave
                                                for(var i=0; i<num; i++) {
                                                    while(i>=count+step)
                                                        count=count+step;
                                                    if(i<count+pos)
                                                        series.appendDataPoint(amp);
                                                    else
                                                        series.appendDataPoint(0);
                                                }
                                                break;
                                            }
                                            case 0x1: { // constant
                                                for(var i=0; i<num; i++)
                                                    series.appendDataPoint(amp);
                                                break;
                                            }
                                            case 0x2: { // sinusoid
                                                for(var i=0; i<num; i++)
                                                    series.appendDataPoint(0.5*amp*(1+Math.sin(2*Math.PI*i*N/num)));
                                                break;
                                            }
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                    }
                }

                Component.onCompleted: {
                    for(var i=0; i<model; i++){
                        var item = itemAt(i);
                        settings[i] = [];
                        settings[i][0] = item.waveType;
                        settings[i][1] = item.amp;
                        settings[i][2] = item.period;
                        settings[i][3] = item.duty;
                    }
                    daParameters.parameters = settings;
                }
            }

            Repeater {
                model: content.columns

                Item {
                    height: 10
                }
            }
        }
    }

    Popup {
        id: settingsPopup
        x: (window.width - width) / 2
        y: window.height / 6
        width: Math.min(window.width, window.height) / 3 * 2
        height: settingsColumn.implicitHeight + topPadding + bottomPadding
        modal: true
        closePolicy: Popup.OnEscape | Popup.OnPressOutside

        contentItem: ColumnLayout {
            id: settingsColumn
            spacing: 20

            Label {
                text: "Settings"
                font.bold: true
            }

            Label {
                text: "是否保存输出设置"
                color: "#e41e25"
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            RowLayout {
                spacing: 10

                Button {
                    id: okButton
                    text: "Ok"
                    focus: true
                    onClicked: {
                        esc();
                    }
                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }

                Button {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: {
                        settingsPopup.close();
                        view.pop(startView);
                    }
                    Layout.preferredWidth: 0
                    Layout.fillWidth: true
                }
            }

            Keys.onReturnPressed: {
                esc();
            }
        }

        onFocusChanged: {
            console.log("Popup: focus changed: " + focus);
        }
    }

    function saveSettings() {
        for(var i=0; i<repeater.model; i++){
            var item = repeater.itemAt(i);
            settings[i] = [];
            settings[i][0] = item.waveType;
            settings[i][1] = item.amp;
            settings[i][2] = item.period;
            settings[i][3] = item.duty;
        }

        daParameters.parameters = settings;
        //console.log("saving settings");
    }

    function loadSettings() {
        for(var i=0; i<repeater.model; i++){
            var item = repeater.itemAt(i);
            item.waveType = settings[i][0];
            item.amp = settings[i][1];
            item.period = settings[i][2];
            item.duty = settings[i][3];
        }
    }

    function popup() {
        settingsPopup.open();
        settingsPopup.focus = true;
        //okButton.focus = true;
    }

    function esc() {
        root.saveSettings();
        //settings.style = root.saveSettings();
        settingsPopup.close();
        view.pop(startView);
    }
}
