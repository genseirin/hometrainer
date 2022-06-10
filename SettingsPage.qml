import QtQuick
import QtQuick.Controls
import com.chattymango.settings_connector 1.0

Page {
    height: window.height
    width: window.width

    title: qsTr("Settings")


    Column {
        id: column
        anchors.fill: parent
        anchors.rightMargin: parent.width * 0.1
        anchors.leftMargin: parent.width * 0.1
        anchors.bottomMargin: parent.height * 0.1
        anchors.topMargin: parent.height * 0.1
        spacing: 10

        Row {
            height: 60
            width: parent.width

            Label {
                text: qsTr("Seconds before new exercise")
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.bold: true
                font.pointSize: 16
                height: 60
                width: parent.width * 0.8
            }

            SpinBox {
                id: countdownSpinBox
                height: 50
                width: parent.width * 0.2
                anchors.verticalCenter: parent.verticalCenter
                to: 100
                editable: true
                from: 1
            }

        }

        Row {
            height: 60
            width: parent.width

            Label {
                text: qsTr("Minutes of inactivity to mark a new session in the reports")
                wrapMode: Text.WordWrap
                anchors.verticalCenter: parent.verticalCenter
                verticalAlignment: Text.AlignVCenter
                font.pointSize: 16
                font.bold: true
                height: 60
                width: parent.width * 0.8
            }

            SpinBox {
                id: newSessionGapSpinBox
                height: 50
                width: parent.width * 0.2
                anchors.verticalCenter: parent.verticalCenter
                to: 100
                editable: true
                from: 10
                value: 10
            }

        }

        Row {
            height: 60
            width: parent.width

            Item {
                height: 50
                width: parent.width * 0.8
            }

            Button {
                height: 50
                width: parent.width * 0.2
                text: qsTr("Save")
                checkable: false
                flat: true
                icon.source: "qrc:/icons/save.svg"
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    const data = [
                        {name:"countdown", value:countdownSpinBox.value},
                        {name:"new_session_gap", value:newSessionGapSpinBox.value}
                     ];

                    settings_connector.saveData(JSON.stringify(data));
                }
            }
        }

    }

    Component.onCompleted: {
        settings_connector.getData();
    }

    SettingsConnector {
        id: settings_connector

        onDataSent:
            data => {
                const dataObj = JSON.parse(data);
                for (var key in dataObj) {
                    switch (dataObj[key].name) {
                        case "countdown":
                        countdownSpinBox.value = parseInt(dataObj[key].value);
                        break;
                     case "new_session_gap":
                        newSessionGapSpinBox.value = parseInt(dataObj[key].value);
                        break;
                    }

                }
            }

        onDataSaved: {
            savedPopup.open();
            popupTimer.start();
        }
    }

    Popup {
        id: savedPopup
        x: 100
        y: 100
        width: parent.width - 200
        height: parent.height - 200
        anchors.centerIn: parent
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

        Column {
            spacing: 10
            anchors.centerIn: parent

            Label {
                text: qsTr("The data was saved.")
                font.pixelSize: 36
                font.bold: true
            }
        }
    }

    Timer {
        id: popupTimer
        interval: 2000
        running: false
        repeat: false
        onTriggered: savedPopup.close()
        triggeredOnStart: false
    }
}
