#include "reports_connector.h"

ReportsConnector::ReportsConnector(QObject *parent)
    : QObject{parent}
{
    m_database = new Database(this, "ReportsConnector");
    m_database->open();

    m_report = new Report(this);
}

ReportsConnector::~ReportsConnector()
{
    delete(m_report);
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

void ReportsConnector::requestReports()
{
    QSqlQuery query(m_database->getQSqlDatabase());

    query.prepare("SELECT time, name FROM report INNER JOIN exercise ON exercise.id = report.exercise_id ORDER BY time DESC");

    if (!query.exec()) {
        qDebug() << "Error: " << query.lastError().text() << query.lastQuery();
    } else {
        QJsonObject jsonAll;
        QDateTime lastGroup = QDateTime::currentDateTime();
        QDateTime lastGroupDayOnly;

        QJsonObject jsonDay;

        int dayIndex = 0;
        int exerciseIndex = 0;
        while (query.next()) {

            QDateTime exerciseTime = QDateTime::fromSecsSinceEpoch(query.value(0).toInt());

            Setting* setting = new Setting();
            int setNewSessionTimout = setting->load("new_session_gap").toInt();
            delete(setting);

            if (lastGroup > exerciseTime.addSecs(setNewSessionTimout*60)) {
                if (!jsonDay.empty()) {
                    lastGroupDayOnly = QDateTime::fromString(lastGroup.toString("yyyy-MM-ddT") + "00:00:00", Qt::ISODate);
                    jsonAll.insert(QString::number(dayIndex++),
                                   QJsonObject({
                                                   {"day", lastGroupDayOnly.toLocalTime().toString("dd.MM.yyyy")},
                                                   {"data", jsonDay}
                                               }));
                    jsonDay = QJsonObject({});
                }
            }

            QJsonObject jsonExercise {
                {"time", QDateTime::fromSecsSinceEpoch(query.value(0).toInt()).toString("dd.MM.yyyy, hh:mm")},
                {"name", query.value(1).toString()}
            };
            jsonDay.insert(QString::number(exerciseIndex++), jsonExercise);

            lastGroup = exerciseTime;
        }

        if (!jsonDay.empty()) {
            lastGroupDayOnly = QDateTime::fromString(lastGroup.toString("yyyy-MM-ddT") + "00:00:00", Qt::ISODate);
            jsonAll.insert(QString::number(dayIndex++),
                           QJsonObject({
                                           {"day", lastGroupDayOnly.toLocalTime().toString("dd.MM.yyyy")},
                                           {"data", jsonDay}
                                       }));
            jsonDay = QJsonObject({});
        }

        emit reportSent(ReportsConnector::qJsonObjectToQString(jsonAll));

    }
}

QString ReportsConnector::qJsonObjectToQString(QJsonObject &obj) const
{
    QJsonDocument doc(obj);

    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
