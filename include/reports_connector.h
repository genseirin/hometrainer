#ifndef REPORTCONNECTOR_H
#define REPORTCONNECTOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDateTime>
#include "include/database.h"
#include "include/exercise.h"
#include "include/setting.h"

typedef QMap<QString, QVariant> DbRow;
typedef QList<DbRow> DbRows;

class ReportsConnector : public QObject
{
    Q_OBJECT
public:
    explicit ReportsConnector(QObject *parent = nullptr);
    ~ReportsConnector();

private:
    Database *m_database;
    Report *m_report;

    QString qJsonObjectToQString(QJsonObject &obj) const;

signals:
    void reportSent(QString data);

public slots:
    void requestReports();

};

#endif // REPORTCONNECTOR_H
