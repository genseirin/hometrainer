import QtQuick 2.0
import QtQuick.Controls

Page {
    id: aboutPage
    title: qsTr("About")
    height: window.height
    width: window.width

    Column {
        id: column
        anchors.fill: parent
        spacing: 30
        anchors.leftMargin: 20
        anchors.bottomMargin: 20
        anchors.topMargin: 20

        Text {
            id: text1
            text: qsTr("About")
            font.pixelSize: 18
            horizontalAlignment: Text.AlignHCenter
            font.bold: true
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Text {
            id: text3
            width: parent.width-20
            text: qsTr("Version: 1.0")
            font.pixelSize: 16
        }

        Text {
            id: text2
            width: parent.width-20
            text: qsTr("Author: Christoph Amthor, www.c-amthor.de, © 2022")
            font.pixelSize: 16
        }

        Text {
            id: text3
            width: parent.width-20
            text: qsTr("License: GPLv3")
            font.pixelSize: 16
        }
    }

}
