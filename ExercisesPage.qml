import QtQuick
import QtQuick.Controls
import com.chattymango.exercises_connector 1.0

Page {
    id: exercisesPage
    title: qsTr("Exercises")
    height: window.height
    width: window.width

    property bool listViewDone: false

    Column {
        anchors.fill: parent

        Rectangle {
            width: parent.width
            height: 60
            color: "transparent"

            Button {
                id: newButton
                font.pixelSize: 22
                icon.source: "qrc:/icons/plus.svg"
                display: AbstractButton.IconOnly
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.topMargin: 10
                background: Rectangle {
                    radius: 5
                    color: parent.down ? "#bbbbbb" :
                                         (parent.hovered ? "#d6d6d6" : "transparent")
                }
                onClicked: exercises_connector.createExercise()
            }

        }

            ListView {
                id: reportList
                spacing: 5
                width: parent.width - 40
                height: exercisesPage.height - 60
                anchors.left: parent.left
                anchors.leftMargin: 20

                model: ListModel {
                    id: reportModel
                }

                delegate: Rectangle {
                    id: reportDelegate
                    height: 50
                    radius: 5
                    color: "#eee"
                    width: reportList.width

                    Row {
                        spacing: 5
                        anchors.fill: parent
                        anchors.leftMargin: 5
                        anchors.bottomMargin: 5
                        anchors.topMargin: 5
                        anchors.rightMargin: 5
                        height: 50

                        property int id: 0

                        SpinBox {
                            id: positionField
                            font.pointSize: 14
                            height: 50
                            width: parent.width * 0.1
                            value: position
                            from: 1
                            to: maxPosition
                            onValueModified: exercises_connector.updateExercise(id, "position", positionField.value)
                        }
                        Switch {
                            id: activeSwitch
                            text: ""
                            height: 50
                            width: parent.width * 0.05
                            checked: active
                            onToggled: exercises_connector.updateExercise(id, "active", activeSwitch.checked?1:0)
                        }
                        TextField {
                            id: nameField
                            text: name
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 16
                            font.bold: true
                            height: 50
                            width: parent.width * 0.20
                            placeholderText: qsTr("Name")
                            onEditingFinished: exercises_connector.updateExercise(id, "name", nameField.text)
                            onTextEdited: descriptionField.color="darkslateblue"
                        }
                        TextField {
                            id: descriptionField
                            text: description
                            verticalAlignment: Text.AlignVCenter
                            font.pointSize: 14
                            height: 50
                            width: parent.width * 0.25
                            placeholderText: qsTr("Description")
                            onEditingFinished: exercises_connector.updateExercise(id, "description", descriptionField.text)
                            onTextEdited: descriptionField.color="darkslateblue"
                        }
                        SpinBox {
                            id: amountField
                            height: 50
                            width: parent.width * 0.15
                            font.pointSize: 14
                            value: amount
                            editable: true
                            from: 1
                            to: 500
                            onValueModified: exercises_connector.updateExercise(id, "amount", amountField.value)
                        }
                        ComboBox {
                            id: typeComboBox
                            height: 50
                            width: parent.width * 0.15
                            font.pointSize: 14
                            textRole: "text"
                            model: ListModel {
                                id: typeComboBoxModel
                                ListElement { key: "repeat"; text: qsTr("times") }
                                ListElement { key: "sustain"; text: qsTr("minutes") }
                            }
                            onCurrentTextChanged: {
                                if (listViewDone) {
                                    exercises_connector.updateExercise(id, "type", typeComboBox.currentIndex)
                                }
                            }
                            Component.onCompleted: {
                                if (type === "repeat") {
                                    currentIndex = 0;
                                } else {
                                    currentIndex = 1;
                                }
                            }
                        }
                        DelayButton {
                            delay: 3000
                            height: 50
                            width: parent.width * 0.05
                            opacity: 0.8
                            contentItem:Image{
                                source: "qrc:/icons/trash.svg"
                                height: 15
                                width: 15
                                fillMode: Image.PreserveAspectFit
                            }
                            onActivated: exercises_connector.deleteExercise(id)
                        }

                    }
                }
            }
    }


    Component.onCompleted: {
        exercises_connector.getExercises();
    }


    ExercisesConnector {
        id: exercises_connector

        function sortObject(obj) {
            return Object.keys(obj).sort().reduce(function (result, key) {
                result[key] = obj[key];
                return result;
            }, {});
        }

        function updateExercise(id, name, value) {
            if (!id) {return;}

            let position;
            if (name === "position") {
                for(let i = 0; i < reportModel.count; i++) {
                    const obj1 = reportModel.get(i);
                    if (obj1.id === id) {
                        position = obj1[name];
                        obj1[name] = value;
                        reportModel.set(i, obj1);
                    }
                }
                for(let j = 0; j < reportModel.count; j++) {
                    const obj2 = reportModel.get(j);
                    if (obj2.id !== id && obj2[name] === value) {
                        obj2[name] = position;
                        reportModel.set(j, obj2);
                        break;
                    }
                }
            } else {
                let found = false;
                for(let k = 0; k < reportModel.count; k++) {
                    const obj3 = reportModel.get(k);
                    if (obj3.id === id) {
                        found = true;
                        if (name === "type") {
                            if (value === 1) {
                                obj3.type = "sustain";
                            } else {
                                obj3.type = "repeat";
                            }
                        } else {
                            obj3[name] = value;
                        }
                        reportModel.set(k, obj3);
                        break;
                    }
                }
                if (!found) {
                    return;
                }
            }


            let returnArray = [];
            for(let l = 0; l < reportModel.count; l++) {
                returnArray.push(reportModel.get(l));
            }
            updateExercises(JSON.stringify(returnArray));
        }

        onDataSent:
            data => {
                listViewDone = false;
                const dataObj = sortObject(JSON.parse(data));
                const maxPosition = Object.keys(dataObj).length;
                reportModel.clear();
                for (var key in dataObj) {
                    let type;
                    reportModel.append({
                                           "id": dataObj[key].id,
                                           "position": dataObj[key].position,
                                           "active": dataObj[key].active,
                                           "name": dataObj[key].name,
                                           "description": dataObj[key].description,
                                           "amount": dataObj[key].amount,
                                           "type": dataObj[key].type,
                                           "maxPosition": maxPosition,
                                       });
                }
                comboTimer.start();
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

    Timer {
        id: comboTimer
        interval: 1000
        running: false
        repeat: false
        onTriggered: ()=>{listViewDone = true}
        triggeredOnStart: false
    }



}
