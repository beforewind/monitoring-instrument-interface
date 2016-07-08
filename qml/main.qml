import QtQuick 2.5
import QtQuick.Layouts 1.1
import Qt.labs.controls 1.0
import QtQuick.Controls 1.4 as Control
import QtQuick.Controls.Styles 1.4 as ControlStyles

import "./Views"

ApplicationWindow {
    id: window
    width:  1024
    height: 768
    visible: true
    title: qsTr("移动式气体检测仪")
    //visibility: "FullScreen"
    //flags: Qt.FramelessWindowHint

    header: Rectangle {
        id: banner
        height: 80
        color: bannerColors[indx]

        property int indx: view.indx
        property var bannerColors: ["#2E2E2E", "cadetblue", "midnightblue", "maroon", "darkmagenta", "darkslategrey", "sienna", "lightskyblue"]
        property var bannerText: ["移动式气体检测仪", "AD数据显示", "DA输出设置", "DA输出显示", "实时气体浓度", "采集系统设置", "设备状态显示"]

        Item {
            id: textItem
            width: titleText.width
            height: titleText.height
            anchors.horizontalCenter: banner.horizontalCenter
            anchors.verticalCenter: banner.verticalCenter

            Text {
                id: titleText
                anchors.verticalCenter: textItem.verticalCenter
                color: "#ffffff"
                font.family: "FangSong"
                font.bold: true
                font.pointSize: banner.height * 0.45
                text: banner.bannerText[banner.indx]
            }
        }

        RowLayout {
            anchors {
                verticalCenter: banner.verticalCenter
                left: banner.left
                leftMargin: 20
            }

            spacing: 20

            Loader {
                sourceComponent: bannerButton
                property string buttonText: (function () {
                    switch(view.currentItem) {
                    case adView: case daOutputView:
                        return "更新通道设置";
                    default:
                        return "";
                    }
                }())

                property bool isVisible: view.currentItem === daOutputView || view.currentItem === adView

                function func() {
                    switch(view.currentItem) {
                    case adView:
                        adView.update();
                        break;
                    case daOutputView:
                        daOutputView.update();
                        break;
                    }
                }
            }

            /*
            Loader {
                sourceComponent: bannerButton
                property string buttonText: "取消"
                property bool isVisible: view.currentItem === daSettingsView

                function func() {
                    console.log("取消");
                    view.pop(startView);
                }
            }
            */
        }

        Loader {
            sourceComponent: bannerButton
            anchors {
                verticalCenter: banner.verticalCenter
                right: banner.right
                rightMargin: 20
            }
            property string buttonText: ( function() {
                switch(view.currentItem){
                case startView:
                    return "关闭";
                //case daSettingsView:
                //    return "取消";
                default:
                    return "返回";
                }}())
            property bool isVisible: true

            function func() {
                switch(view.currentItem){
                case startView:
                    Qt.quit();
                    break;
                case daSettingsView:
                    daSettingsView.popup();
                    break;
                default:
                    view.pop(startView);
                    break;
                }
            }
        }

        Component {
            id: bannerButton

            Pane {
                background: Rectangle {
                    color: banner.bannerColors[banner.indx]
                }

                Button {
                    id: control
                    text: buttonText
                    anchors {
                        verticalCenter: parent.verticalCenter
                        horizontalCenter: parent.horizontalCenter
                    }
                    visible: isVisible
                    onClicked: func();
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
                }
            }
        }
    }

    StackView {
        id: view
        anchors.fill: parent
        initialItem: startView

        property int indx: 0

        onCurrentItemChanged: {
            switch(currentItem) {
            case startView:
                indx = 0;
                break;
            case adView:
                indx = 1;
                break;
            case daSettingsView:
                indx = 2;
                break;
            case daOutputView:
                indx = 3;
                break;
            case gasView:
                indx = 4;
                break;
            case settingsView:
                indx = 5;
                break;
            case pipeView:
                indx = 6;
                break;
            }
        }
    }

    Item {
        visible: false

        StartView {
            id: startView
        }

        DAQChartViews {
            id: adView
            source: adsource
            saveVisible: true
        }

        DASettingsView {
            id: daSettingsView
        }

        DAQChartViews {
            id: daOutputView
            source: dasource
        }

        GasView {
            id: gasView
        }

        SettingsView {
            id: settingsView
        }

        PipeView {
            id: pipeView
        }
    }

    QtObject{
        id: sampleStatus

        property bool refresh: false
        property real refreshRate: 1 / 10 * 1000 // 10 Hz
        onRefreshChanged: {
            device.status = refresh;
            console.log("status: refresh changed: " + refresh);
        }
    }

    QtObject {
        id: devDOStatus
        property var relayChannel: [6, 7, 12, 13, 14, 15]
        property var pumpChannel: [1, 3, 5]
        property var heatChannel: [1, 3, 5]
    }

    Connections {
        target: device
        onStatusChanged: {
            sampleStatus.refresh = device.status;
        }
    }

    Text {
        text: view.depth
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    FontLoader {
        id: fontUbuntu
        source: "qrc:/content/fonts/Ubuntu-R.ttf"
    }

    FontLoader {
        id: fontOpenSans
        source: "qrc:/content/fonts/OpenSans-Regular.ttf"
    }
}
