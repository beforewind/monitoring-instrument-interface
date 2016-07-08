import QtQuick 2.0
import QtQuick.Layouts 1.1
import Qt.labs.controls 1.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Extras 1.4 as Extras

Item {
    Item {
        anchors.fill: parent
        anchors.margins: 20

        RowLayout {
            spacing: 40

            ColumnLayout {
                //anchors.top: parent.top
                //anchors.left: parent.left
                //anchors.margins: 20
                GroupBox {
                    title: "采集设置"

                    ColumnLayout {
                        Label { text: "状态： " + (sampleStatus.refresh ? "正在采集" : "停止采集") }

                        RowLayout {
                            Label {
                                text: "采集周期"
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                            }
                            ComboBox {
                                id: samplePeriod
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

                                currentIndex: 1
                                model: [10, 100, 1000]

                                onCurrentIndexChanged: {
                                    device.setSamplePeriod(model[currentIndex]);
                                }

                            }
                            Label { text: "ms" }
                        }

                        RowLayout {
                            Loader {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                sourceComponent: buttonComponent
                                property string buttonText: "开始采集"

                                function func() {
                                    sampleStatus.refresh = true;
                                    console.log(qsTr("开始采集"));
                                }
                            }

                            Loader {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                sourceComponent: buttonComponent
                                property string buttonText: "停止采集"

                                function func() {
                                    sampleStatus.refresh = false;
                                    console.log("停止采集");
                                }
                            }
                        }
                    }
                }

                GroupBox {
                    title: "数据库导出"
                    width: 300
                    //Layout.implicitWidth: 300

                    ColumnLayout {
                        Text {
                            text: "当前数据库: " + database.tableName
                            font.pixelSize: 14
                            font.family: fontOpenSans.name
                        }

                        RowLayout {
                            Loader {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                sourceComponent: buttonComponent
                                property string buttonText: "刷新列表"

                                function func() {
                                    database.loadTables();
                                }
                            }

                            Loader {
                                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                                sourceComponent: buttonComponent
                                property string buttonText: "导出"

                                function func() {
                                    database.exportFile(database.tables[listViewTumbler.currentIndex], fileFormat.currentText);
                                    //console.log("导出");
                                }
                            }

                            Label {
                                text: "文件格式"
                            }

                            ComboBox {
                                id: fileFormat
                                model: ["xlsx", "txt"]
                            }
                        }

                        Frame {
                            Layout.fillWidth: true
                            Tumbler {
                                id: listViewTumbler
                                anchors.left: parent.left
                                anchors.right: parent.right

                                model: database.tables
                                visibleItemCount: 5
                                delegate: Text {
                                    id: label
                                    text: modelData
                                    color: ListView.isCurrentItem ? "blcak" : "#666666"
                                    font.family: fontOpenSans.name
                                    font.pixelSize: 12
                                    opacity: 0.4 + Math.max(0, 1 - Math.abs(Tumbler.displacement)) * 0.6
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }

                                contentItem: ListView {
                                    anchors.fill: parent
                                    model: listViewTumbler.model
                                    delegate: listViewTumbler.delegate
                                    highlight: Frame { }

                                    snapMode: ListView.SnapToItem
                                    highlightRangeMode: ListView.StrictlyEnforceRange
                                    preferredHighlightBegin: height / 2 - (height / listViewTumbler.visibleItemCount / 2)
                                    preferredHighlightEnd: height / 2  + (height / listViewTumbler.visibleItemCount / 2)
                                    clip: true
                                }
                            }
                        }
                    }
                }
            }

            ColumnLayout {
                GroupBox {
                    title: "电磁阀开关设定"

                    ColumnLayout {

                        Repeater {
                            id: relay
                            model: 6

                            Switch {
                                text: "电磁阀" + index
                                checked: false
                                onCheckedChanged: {
                                    device.setDO(devDOStatus.relayChannel[index], checked);
                                }
                                Component.onCompleted: {
                                    device.setDO(devDOStatus.relayChannel[index], checked);
                                }
                            }
                        }
                    }
                }

                GroupBox {
                    title: "气泵开关设定"

                    ColumnLayout {

                        Repeater {
                            id: pump
                            model: 3

                            Switch {
                                text: "气泵" + index
                                checked: false
                                onCheckedChanged: {
                                    device.setDO(devDOStatus.pumpChannel[index], checked);
                                }
                                Component.onCompleted: {
                                    device.setDO(devDOStatus.pumpChannel[index], checked);
                                }
                            }
                        }
                    }
                }

                GroupBox {
                    title: "温控设定"

                    ColumnLayout {

                        Repeater {
                            id: heater
                            model: 2

                            RowLayout {
                                Label {
                                    text: "温控" + index
                                }

                                SpinBox {
                                    id: heatSpin
                                    value: 25
                                    from: 0
                                    to: 50
                                }

                                Button {
                                    text: "设定"
                                    onClicked: {
                                        device.setControl(index, heatSpin.value);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: buttonComponent

        Pane {
            /*
            background: Rectangle {
                color: banner.colors[banner.indx]
            }
            */

            Button {
                id: button
                text: buttonText
                anchors {
                    verticalCenter: parent.verticalCenter
                    horizontalCenter: parent.horizontalCenter
                }
                onClicked: func();
                /*
                label: Text {
                    x: control.leftPadding
                    y: control.topPadding
                    width: control.availableWidth
                    height: control.availableHeight
                    text: control.text
                    font: control.font
                    opacity: enabled || highlighted ? 1 : 0.3
                    color: control.highlighted ? "#FFFFFF" : (control.pressed ? "#BCBCBC" : "#E5E5E5")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }
                */
            }
        }
    }
}
