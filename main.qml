import QtQuick
import QtQuick.Controls
import QtQuick3D 6.2
//import com.chattymango.home_connector 1.0

ApplicationWindow {
    id: window
    visible: true
    minimumHeight: 500
    minimumWidth: 900
    height: 600
    width: 1000
    title: qsTr("Home Trainer")

    menuBar: ToolBar {
        contentHeight: menuButton.implicitHeight
        Row {
            id: row
            anchors.fill: parent;
            spacing: 5

            ToolButton {
                id: menuButton
                text: stackView.depth > 1 ? "\u25C0" : "\u2630"
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                onClicked: {
                    if (stackView.depth > 1) {
                        stackView.pop();
                    } else {
                        drawer.open()
                    }
                }
            }
        }
        Label {
            text: stackView.currentItem.title ? stackView.currentItem.title : ""
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            verticalAlignment: Text.AlignVCenter
            anchors.rightMargin: 10
            anchors.right: parent.right
            height: menuButton.implicitHeight
        }

    }

    Drawer {
        id: drawer
        width: window.width * 0.33
        height: window.height
        //        onAboutToShow: {}

        Column {
            anchors.fill: parent

            ItemDelegate {
                id: itemReports
                text: qsTr("Reports")
                font.pointSize: 18
                width: parent.width
                icon.source: "qrc:/icons/report.svg";
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    stackView.push("ReportsPage.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                id: itemExercises
                text: qsTr("Exercises")
                font.pointSize: 18
                width: parent.width
                icon.source: "qrc:/icons/exercises.svg";
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    stackView.push("ExercisesPage.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                id: itemSettings
                text: qsTr("Settings")
                font.pointSize: 18
                width: parent.width
                icon.source: "qrc:/icons/settings.svg";
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    stackView.push("SettingsPage.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                id: itemAbout
                text: qsTr("About")
                font.pointSize: 18
                width: parent.width
                icon.source: "qrc:/icons/info.svg";
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    stackView.push("AboutPage.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomePage.qml"
        anchors.fill: parent
    }

}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
