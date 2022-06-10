import QtQuick
import QtQuick.Controls
import QtMultimedia
import com.chattymango.home_connector 1.0

Page {
    property bool sessionRunning: false;
    property int countdown: 3;
    property int countdownTimerDuration: 10;
    property var nextExercise: ({})
    property string countdownWaitDefaultColor: "black"
    property string countdownWaitUrgencyColor: "red"
    property string countdownDoDefaultColor: "black"
    property string countdownDoUrgencyColor: "green"

    id: homePage
    title: qsTr("")
    height: window.height
    width: window.width

    Column {
        id: column
        width: parent.width - 40
        height: parent.height - 40
        anchors.top: parent.top
        spacing: 20
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 20

        Rectangle {
            id: endContainer
            visible: false
            height: 50
            width: parent.width
            radius: 5
            color: "transparent"

            Button {
                id: endButton
                text: qsTr("Pause")
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Qt.application.font.pixelSize * 1.2
                flat: true
                highlighted: false
                height: 40
                width: 100
                anchors.right: parent.right
                anchors.rightMargin: 20
                icon.source: "qrc:/icons/pause.svg"
                display: AbstractButton.TextBesideIcon
                onClicked: {
                    homeConnector.pauseSession()
                }
            }
        }

        Rectangle {
            id: startContainer
            visible: true
            height: parent.height - 70
            width: parent.width
            color: "#e4e4e4"
            radius: 5

            Button {
                id: startButton
                text: "<br>"+qsTr("Click here to start a session")
                font.pixelSize: Qt.application.font.pixelSize * 1.6
                flat: true
                highlighted: false
                height: parent.height - 100
                width: parent.width - 100
                anchors.centerIn: parent
                onClicked: homeConnector.startSession()
                icon.source: "qrc:/icons/start.svg"
                icon.width: 100
                display: AbstractButton.TextUnderIcon
                padding: 50
            }
        }

        Rectangle {
            id: sessionContainer
            color: "#e4e4e4"
            radius: 5
            height: 200
            width: parent.width
            visible: false

            Row {
                id: waitingForExerciseBlock
                visible: false
                spacing: 20
                anchors.fill: parent
                anchors.margins: 10
                padding: 20

                Column {
                    id: previewExercise
                    spacing: 5
                    width: parent.width * 0.55
                    height: 180

                    Label {
                        text: qsTr("next exercise:")
                        height: 20
                        width: parent.width
                        font.pixelSize: 20
                    }
                    Text {
                        id: previewExerciseName
                        text: ""
                        font.pixelSize: 36
                        font.bold: true
                        height: 50
                        width: parent.width
                    }
                    Text {
                        id: previewExerciseDescription
                        text: ""
                        font.pixelSize: 18
                        height: 60
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        id: previewExerciseMode
                        text: ""
                        font.pixelSize: 16
                        height: 40
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }

                Rectangle {
                    height: 150
                    width: parent.width * 0.20
                    color: "transparent"

                    Image {
                        id: waitingIcon
                        source: "qrc:/icons/hourglass.svg"
                        height: 70
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }

                Label {
                    id: countdownLabel
                    text: ""
                    font.pixelSize: 120
                    font.bold: true
                    height: 150
                    width: parent.width * 0.20
                    color: countdownWaitDefaultColor
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Row {
                id: doingExerciseBlock
                visible: false
                spacing: 20
                anchors.fill: parent
                anchors.margins: 10
                padding: 20

                Column {
                    spacing: 5
                    width: parent.width * 0.55
                    height: 180

                    Label {
                        text: qsTr("now:")
                        height: 20
                        width: parent.width
                        font.pixelSize: 20
                    }
                    Text {
                        id: doingExerciseName
                        text: ""
                        font.pixelSize: 36
                        font.bold: true
                        height: 50
                        width: parent.width
                    }
                    Text {
                        id: doingExerciseDescription
                        text: ""
                        font.pixelSize: 18
                        height: 60
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                    Text {
                        id: doingExerciseMode
                        text: ""
                        font.pixelSize: 16
                        height: 40
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }

                }

                Rectangle {
                    height: 150
                    width: parent.width * 0.20
                    color: "transparent"


                    Image {
                        id: doingIcon
                        source: ""
                        height: 70
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                    }
                }

                Label {
                    id: doingExerciseCountdown
                    text: ""
                    visible: false
                    font.pixelSize: 120
                    font.bold: true
                    width: parent.width * 0.20
                    height: 150
                    wrapMode: Text.WordWrap
                    color: countdownDoDefaultColor
                }

                Rectangle {
                    width: parent.width * 0.15
                    height: 150
                    color: "transparent"

                    Button {
                        id: nextExerciseButton
                        visible: false
                        text: qsTr("Done")
                        font.pixelSize: 22
                        width: 120
                        height: 60
                        onClicked: homeConnector.finishOngoingExercise()
                        icon.source: "qrc:/icons/done.svg"
                        display: AbstractButton.TextBesideIcon
                        anchors.centerIn: parent
                        background: Rectangle {
                            radius: 5
                            color: parent.down ? "#bbbbbb" :
                                                 (parent.hovered ? "#d6d6d6" : "transparent")
                        }

                    }

                }

            }
        }

        Row {
            height: 500
            width: parent.width - 20
            spacing: 20

            Column {
                height: 500
                width: parent.width / 2
                id: remainingContainer
                visible: false
                spacing: 10

                Rectangle {
                    color: "#e4e4e4"
                    radius: 5
                    height: 50
                    width: parent.width
                    z: 100

                    Label {
                        text: qsTr("Next up")
                        font.pointSize: 15
                        font.bold: true
                        styleColor: "#f95030"
                        anchors.centerIn: parent
                    }
                }


                ListView {
                    id: remainingExerciseList
                    spacing: 5
                    height: window.height - 200
                    width: parent.width
                    z: 99
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: false

                    model: ListModel {
                        id: remainingExerciseModel
                    }

                    delegate: Rectangle {
                        id: remainingExerciseDelegate
                        height: 80
                        radius: 5
                        color: "#eee"
                        width: remainingContainer.width


                        Column {
                            spacing: 3
                            anchors.fill: parent
                            anchors.leftMargin: 5
                            anchors.bottomMargin: 5
                            anchors.topMargin: 5

                            Text {
                                text: name
                                font.bold: true
                                font.pointSize: 24
                                height: 30
                                width: parent.width
                            }
                            Text {
                                text: description
                                wrapMode: Text.WordWrap
                                font.pointSize: 12
                                height: 30
                                width: parent.width
                            }
                        }
                    }
                }
            }

            Column {
                height: 500
                width: parent.width / 2
                id: doneContainer
                visible: false
                spacing: 10

                Rectangle {
                    color: "#e4e4e4"
                    radius: 5
                    height: 50
                    width: parent.width
                    z: 100

                    Label {
                        text: qsTr("Done")
                        font.pointSize: 15
                        font.bold: true
                        styleColor: "#f95030"
                        anchors.centerIn: parent
                    }
                }

                ListView {
                    id: doneExerciseList
                    spacing: 5
                    height: window.height - 200
                    width: parent.width
                    z: 99
                    anchors.left: parent.left
                    anchors.right: parent.right
                    visible: false

                    model: ListModel {
                        id: doneExerciseModel
                    }

                    delegate: Rectangle {
                        id: doneExerciseDelegate
                        height: 80
                        radius: 5
                        color: "#eee"
                        width: doneContainer.width

                        Column {
                            spacing: 5
                            anchors.fill: parent
                            anchors.leftMargin: 5
                            anchors.bottomMargin: 5
                            anchors.topMargin: 5

                            Text {
                                text: name
                                font.bold: true
                                font.pointSize: 24
                                height: 30
                                width: parent.width
                            }
                            Text {
                                text: description
                                wrapMode: Text.WordWrap
                                font.pointSize: 12
                                height: 30
                                width: parent.width
                            }
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        homeConnector.sendInitialData();
    }

    HomeConnector {
        id: homeConnector

        function sortObject(obj) {
            return Object.keys(obj).sort().reduce(function (result, key) {
                result[key] = obj[key];
                return result;
            }, {});
        }

        function updateCountdown() {
            if (countdown < 0) {
                countdownTimer.stop();
                return false;
            }

            countdownLabel.text = countdown;
            if (countdown > 3) {
                countdownLabel.color = countdownWaitDefaultColor;
            } else {
                countdownLabel.color = countdownWaitUrgencyColor;
            }

            if (countdown == 0) {
                countdown = -1;
                countdownTimer.stop();
                waitingForExerciseBlock.visible = false;
                startNextExercise();
                return false;
            }
            countdown--;
        }

        function renderExercises(dataObj) {
            let index;
            const done = sortObject(dataObj.done);
            const now = dataObj.now;
            const remaining = sortObject(dataObj.remaining);

            doneExerciseModel.clear();
            remainingExerciseModel.clear();

            for (index in done) {
                doneExerciseModel.append({
                                             "name": done[index].name,
                                             'description': done[index].description
                                         });
            }

            if (Object.keys(now).length) {
                doingExerciseName.text = now.name;
                doingExerciseDescription.text = now.description;
                if (now.type === "sustain") {
                    doingExerciseMode.text = qsTr("for %1 seconds").arg(now.amount);
                    doingIcon.source = "qrc:/icons/clock.svg";
                } else if (now.type === "repeat") {
                    doingExerciseMode.text = qsTr("repeat %1 times").arg(now.amount);
                    doingIcon.source = "qrc:/icons/redo.svg";
                }   else {
                    doingExerciseMode.text = "";
                    doingIcon.source = "qrc:/icons/exercises.svg";
                }

                if (now.type === "sustain") {
                    doingExerciseCountdown.visible = true;
                    nextExerciseButton.visible = false;
                } else {
                    doingExerciseCountdown.visible = false;
                    nextExerciseButton.visible = true;
                }
            }


            for (index in remaining) {
                remainingExerciseModel.append({
                                                  "name": remaining[index].name,
                                                  "description": remaining[index].description
                                              });
            }
            nextExercise = remaining[Object.keys(remaining)[0]];
            setVisibilities();
        }

        function setVisibilities() {

            if (sessionRunning && doneExerciseModel.count) {
                doneExerciseList.visible = true
            } else {
                doneExerciseList.visible = false
            }

            if (sessionRunning && remainingExerciseModel.count) {
                remainingExerciseList.visible = true
            } else {
                remainingExerciseList.visible = false
            }
        }

        function previewNext() {
            waitingForExerciseBlock.visible = true;
            previewExerciseName.text = nextExercise.name;
            previewExerciseDescription.text = nextExercise.description;
            if (nextExercise.type === "repeat") {
                previewExerciseMode.text = qsTr("repeat %1 times").arg(nextExercise.amount);
            } else if (nextExercise.type === "sustain") {
                previewExerciseMode.text = qsTr("for %1 seconds").arg(nextExercise.amount);
            }
        }

        onInitialDataSent:
            data => {
                const dataObj = JSON.parse(data);
                renderExercises(dataObj);
            }

        onSessionStarted:
            (data, setTimerDuration) =>
            {
                countdownTimerDuration = setTimerDuration;

                sessionRunning = true;

                endContainer.visible = true;
                startContainer.visible = false;
                sessionContainer.visible = true;
                waitingForExerciseBlock.visible = true;
                remainingContainer.visible = true;
                doneContainer.visible = true;

                const dataObj = JSON.parse(data);
                renderExercises(dataObj);

                previewNext();
                countdown = countdownTimerDuration;
                countdownLabel.text = countdownTimerDuration;
                countdownLabel.color = countdownWaitDefaultColor
                countdownTimer.start();
            }

        onExerciseStarted:
            data =>
            {
                doingExerciseBlock.visible = true;
                waitingForExerciseBlock.visible = false;
                const dataObj = JSON.parse(data);
                renderExercises(dataObj);
            }

        onExerciseFinished:
            data => {
                playMusic.play();
                const dataObj = JSON.parse(data);
                renderExercises(dataObj);
                waitingForExerciseBlock.visible = true;
                doingExerciseBlock.visible = false;

                previewNext();
                countdown = countdownTimerDuration;
                countdownLabel.text = countdownTimerDuration;
                countdownLabel.color = countdownWaitDefaultColor
                countdownTimer.start();

            }

        onTimerUpdate:
            seconds => {
                if (seconds < 1) {
                    return;
                }

                doingExerciseCountdown.text = seconds;
                if (seconds > 3) {
                    doingExerciseCountdown.color = countdownDoDefaultColor;
                } else {
                    doingExerciseCountdown.color = countdownDoUrgencyColor;
                }
            }

        onSessionFinished:
            regular => {
                countdownTimer.stop();
                if (regular) {
                    successPopup.open();
                } else {
                    abortPopup.open();
                }

                endContainer.visible = false;
                startContainer.visible = true;
                sessionContainer.visible = false;
                remainingContainer.visible = false;
                doneContainer.visible = false;
                doingExerciseBlock.visible = false;
                sessionRunning=false;
            }

        onSessionPaused:
        {
            countdownTimer.stop();
            pausePopup.open();
        }

        onSessionResumed:
        {

        }
    }

    Popup {
        id: pausePopup
        x: 100
        y: 100
        width: parent.width - 200
        height: parent.height - 200
        anchors.centerIn: parent
        modal: true
        focus: true
        closePolicy: Popup.NoAutoClose

        Column {
            id: column1
            spacing: 20
            anchors.centerIn: parent

            Label {
                text: qsTr("The session was paused")
                font.pixelSize: 28
                font.bold: true
            }
            Row {
                spacing: 20
                height: 30
                anchors.horizontalCenter: parent.horizontalCenter

                Button {
                    text: qsTr("Resume")
                    font.pixelSize: 24
                    font.bold: true
                    icon.source: "qrc:/icons/start.svg"
                    display: AbstractButton.TextBesideIcon
                    onClicked: {
                        homeConnector.resumeSession();
                        countdownTimer.start();
                        pausePopup.close();
                    }
                }
                Button {
                    text: qsTr("Quit")
                    font.pixelSize: 24
                    font.bold: true
                    icon.source: "qrc:/icons/exit.svg"
                    display: AbstractButton.TextBesideIcon
                    onClicked: {
                        homeConnector.finishSession();
                        pausePopup.close();
                    }
                }
            }
        }
    }

    Popup {
        id: successPopup
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

            Rectangle {
                color: "transparent"
                width: parent.width
                height: parent.height * 0.6

                Image {
                    source: "qrc:/icons/smile.svg"
                    height: 70
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Label {
                text: qsTr("Excellent, you made it!")
                font.pixelSize: 72
                font.bold: true
            }
        }
    }

    Popup {
        id: abortPopup
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

            Rectangle {
                color: "transparent"
                width: parent.width
                height: parent.height * 0.6

                Image {
                    source: "qrc:/icons/meh.svg"
                    height: 70
                    fillMode: Image.PreserveAspectFit
                    anchors.centerIn: parent
                }
            }

            Label {
                text: qsTr("The session was quit")
                font.pixelSize: 36
                font.bold: true
            }
        }
    }

    Timer {
        id: countdownTimer
        interval: 1000
        running: false
        repeat: true
        onTriggered: homeConnector.updateCountdown()
        triggeredOnStart: true
    }

    SoundEffect {
        id: playMusic
        source: ":/sounds/sicko-mode-sound-effect.wav"
    }

}




