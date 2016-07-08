import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: root

    GridLayout {
        anchors.fill: parent
        anchors.margins: 40
        columns: 5

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "系统设置"
            property bool icon: true
            property string iconPath: "qrc:/content/images/Control Panel.png"
            function action() {
                //adView.loadSettings();
                view.push(settingsView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "设备状态显示"
            property bool icon: true
            property string iconPath: "qrc:/content/images/USB alt 2.png"
            function action() {
                //adView.loadSettings();
                view.push(pipeView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "AD"
            property bool icon: true
            property string iconPath: "qrc:/content/images/Task Manager alt 1.png"
            function action() {
                adView.loadSettings();
                view.push(adView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "DA输出设置"
            property bool icon: true
            property string iconPath: "qrc:/content/images/Configure1.png"
            function action() {
                daSettingsView.loadSettings();
                view.push(daSettingsView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "DA输出显示"
            property bool icon: true
            property string iconPath: "qrc:/content/images/Gadgets1.png"
            function action() {
                daOutputView.loadSettings();
                view.push(daOutputView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "实时气体浓度"
            property bool icon: true
            property string iconPath: "qrc:/content/images/Network1.png"
            function action() {
                view.push(gasView);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "选择性改变"
            property bool icon: true
            property string iconPath: "qrc:/content/images/report_card-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "不确定度计算"
            property bool icon: true
            property string iconPath: "qrc:/content/images/quill_with_ink-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "故障检测"
            property bool icon: true
            property string iconPath: "qrc:/content/images/heart_monitor-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "故障诊断"
            property bool icon: true
            property string iconPath: "qrc:/content/images/caduceus-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "健康评估"
            property bool icon: true
            property string iconPath: "qrc:/content/images/hearts-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Loader {
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            sourceComponent: metroButton
            property string buttonText: "故障预测"
            property bool icon: true
            property string iconPath: "qrc:/content/images/electrical_threshold-48.png"
            function action() {
                console.log(buttonText);
            }
        }

        Component {
            id: metroButton

            Item {
                id: item
                height: 100
                width: height
                implicitHeight: width
                implicitWidth: implicitHeight

                Rectangle {
                    height: parent.height
                    width: height
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    color: "#297ACC"
                    border.color: "#94BDE6"
                    border.width: 0

                    Text {
                        anchors {
                            left: parent.left
                            bottom: parent.bottom
                            margins: 5
                        }

                        text: buttonText
                        font.family: "Microsoft YaHei UI"
                        font.pixelSize: 12
                        color: "white"
                        antialiasing: true
                    }

                    Image {
                        visible: icon
                        anchors {
                            horizontalCenter: parent.horizontalCenter
                            verticalCenter: parent.verticalCenter
                        }
                        height: 48
                        width: height
                        source: iconPath
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            action();
                        }
                        onPressed: {
                            parent.height = item.height - 4;
                            parent.border.width = 0;
                        }
                        onReleased: {
                            parent.height = item.height;
                            parent.border.width = 2;
                        }

                        hoverEnabled: true
                        onContainsMouseChanged: {
                            if(containsMouse) {
                                parent.border.width = 2;
                            } else {
                                parent.border.width = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}
