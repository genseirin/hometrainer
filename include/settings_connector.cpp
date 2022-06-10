#include "settings_connector.h"

SettingsConnector::SettingsConnector(QObject *parent)
    : QObject{parent}
{
    m_database = new Database(this, "SettingsConnector");
    m_database->open();

    m_setting = new Setting(this);
}

SettingsConnector::~SettingsConnector()
{
    delete(m_setting);
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

void SettingsConnector::getData()
{
    DbRows rows = *m_database->getAll("setting");

    QJsonObject jsonAll;
    int i = 0;

    for (const auto &row : rows) {
        jsonAll.insert(QString::number(i++),
                       QJsonObject({
                                       {"name", row["name"].toString()},
                                       {"value", row["value"].toString()}
                                   }));
    }

    emit dataSent(SettingsConnector::qJsonObjectToQString(jsonAll));
}

void SettingsConnector::saveData(QString data)
{
    QJsonDocument decoded = QJsonDocument::fromJson(data.toUtf8());

    if (!decoded.isNull()) {
        QJsonArray array = decoded.array();
        for (const QJsonValue &item : array) {
            m_setting->save(item.toObject().value("name").toString(), QString::number(item.toObject().value("value").toInt()));
        }
    }
    emit dataSaved();
}

QString SettingsConnector::qJsonObjectToQString(QJsonObject &obj) const
{
    QJsonDocument doc(obj);

    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
