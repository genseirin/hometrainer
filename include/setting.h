#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "include/database.h"

typedef QMap<QString, QVariant> DbRow;
typedef QList<DbRow> DbRows;

class Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = nullptr);
    ~Setting();

    void save(QString name, QString value);
    QString load(QString name);
signals:

private:
    QSqlQuery m_query;
    Database* m_database;

    bool rowExists();
    bool execQuery();
};

#endif // SETTINGS_H
