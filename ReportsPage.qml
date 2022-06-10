import QtQuick
import QtQuick.Controls
import com.chattymango.reports_connector 1.0

Page {
    id: reportsPage
    title: qsTr("Report")
    height: window.height
    width: window.width

    ListView {
        id: reportList
        spacing: 5
        width: parent.width - 40
        height: parent.height - 40
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.topMargin: 20

        model: ListModel {
            id: reportModel
        }

        delegate: Rectangle {
            id: reportDelegate
            height: rowheight
            radius: 5
            color: rowcolor
            width: reportList.width

            Row {
                spacing: 5
                anchors.fill: parent
                anchors.leftMargin: 5
                anchors.bottomMargin: 5
                anchors.topMargin: 5

                Rectangle {
                    visible: dayVisible
                    width: parent.width * 0.33
                    height: rowheight
                    color: "transparent"

                    Image {
                        source: "qrc:/icons/tasks.svg"
                        fillMode: Image.PreserveAspectFit
                        anchors.left: parent.left
                        anchors.top: parent.top
                        anchors.topMargin: (rowheight - height) / 2
                        anchors.leftMargin: 10
                        height: 20
                    }

                    Text {
                        text: day
                        verticalAlignment: Text.AlignVCenter
                        font.bold: true
                        font.pointSize: 24
                        height: rowheight
                        width: parent.width * 0.7
                        anchors.right: parent.right
                    }
                }

                Rectangle {
                    visible: reportIconVisible
                    width: parent.width * 0.33
                    height: rowheight
                    color: "transparent"

                    Image {
                        source: "qrc:/icons/done.svg"
                        fillMode: Image.PreserveAspectFit
                        anchors.centerIn: parent
                        height: 20
                    }
                }
                Text {
                    text: name
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 18
                    height: rowheight
                    width: parent.width * 0.33
                }
                Text {
                    text: time
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 18
                    height: rowheight
                    width: parent.width * 0.33
                }
            }
        }
    }

    Component.onCompleted: {
        reports_connector.requestReports();
    }

    ReportsConnector {
        id: reports_connector

        function sortObject(obj) {
            return Object.keys(obj).sort().reduce(function (result, key) {
                result[key] = obj[key];
                return result;
            }, {});
        }

        function renderReports(dataObj) {
            reportModel.clear();

        }

        onReportSent: data => {
                          const dataObj = sortObject(JSON.parse(data));
                          for(const i in dataObj) {
                              reportModel.append({
                                                     "rowheight": 50,
                                                     "rowcolor": "#ddd",
                                                     "day": dataObj[i].day,
                                                     "name": "",
                                                     "time": "",
                                                     "dayVisible": true,
                                                     "reportIconVisible": false
                                                 })
                              const exercise = sortObject(dataObj[i].data);
                              for(const j in exercise) {
                                  reportModel.append({
                                                         "day": "",
                                                         "rowheight": 40,
                                                         "rowcolor": "#eee",
                                                         "name": exercise[j].name,
                                                         "time": exercise[j].time,
                                                         "dayVisible": false,
                                                         "reportIconVisible": true
                                                     })
                              }
                          }
                      }
    }
}
