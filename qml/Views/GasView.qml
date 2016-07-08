import QtQuick 2.0
import Qt.labs.controls 1.0
import QtQuick.Layouts 1.1
import QtWebEngine 1.2
import QtWebChannel 1.0

Item {
    WebEngineView {
        id: webView
        visible: StackView.Active
        url: "qrc:/content/html/gasview.html"
        anchors.fill: parent

        Component.onCompleted: {
            webChannel.registerObject("gasdata", gasdata);
            webChannel.registerObject("legenddata", legendData);
        }

        onJavaScriptConsoleMessage: {
            console.log("html.js: " + sourceID + "[" + lineNumber + ", " + level + "]: " + message);
        }
    }

    RowLayout {
        Text {
            text: "TEMP: " + gasdata.temp
        }

        Text {
            text: "RH: " + gasdata.rh
        }
    }

    Item {
        id: legend
        anchors {
            left: webView.left
            leftMargin: 60
            top: webView.top
            topMargin: 60
        }

        Item {
            anchors.left: parent.left
            anchors.top: parent.top

            width: 180
            height: 200

            Rectangle {
                anchors.fill: parent
                color: "#fedcbd"
                opacity: 0.4
            }

            ColumnLayout {
                id: layout
                anchors.fill: parent
                anchors.leftMargin: 10
                property int recSize: 25

                Repeater {
                    model: 4

                    RowLayout {
                        Rectangle { color: legendData.colors[index]; height: layout.recSize; width: height}
                        Text{ text: legendData.labels[index]; font.pixelSize: 18; font.family: fontUbuntu.name }
                        Text{ text: "="; font.pixelSize: 18; font.family: fontUbuntu.name }
                        Text{ text: Math.round(legendData.data[index] * 100) / 100; font.pixelSize: 18; font.family: fontUbuntu.name }
                    }
                }
            }
        }
    }

    QtObject{
        id: legendData
        property real so2: 0
        property real no2: 0
        property real o3: 0
        property real co: 0

        property var data: [so2, no2, o3, co]
        property var labels: ["SO2", "NO2", "O3", "CO"]
        property var colors: ["#ff7f50", "#9400d3", "#00ced1", "#dc143c"]

        function setLegend(reading) {
            so2 = reading[0];
            no2 = reading[1];
            o3 = reading[2];
            co = reading[3];
        }
    }
}
