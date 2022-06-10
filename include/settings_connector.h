#ifndef SETTINGSCONNECTOR_H
#define SETTINGSCONNECTOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "include/database.h"
#include "include/setting.h"

class SettingsConnector : public QObject
{
    Q_OBJECT
public:
    explicit SettingsConnector(QObject *parent = nullptr);
    ~SettingsConnector();

private:
    Database *m_database;
    Setting *m_setting;

    QString qJsonObjectToQString(QJsonObject &obj) const;

signals:
    void dataSent(QString data);
    void dataSaved();

public slots:
    void getData();
    void saveData(QString data);

};

#endif // SETTINGSCONNECTOR_H
