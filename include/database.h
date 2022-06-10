#ifndef DATABASE_H
#define DATABASE_H

#define DATABASE_NAME "database.sqlite"

#include <QObject>
#include <QSqlDatabase>
#include <QVariant>
#include <QDebug>
#include <QDir>
#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
//#include <QLatin1String>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(QObject *parent = nullptr, QString connectionName = "");
    ~Database();

    bool open();
    bool close();

    QList<int> getIDs(QString table, QString where = "", QString orderby = "");
    QList<QMap<QString, QVariant>>* getAll(QString table, QString where = "", QString orderby = "");

    const QString &getDbName() const;
    void setDbName(const QString &newDbName);

    const QSqlDatabase &getQSqlDatabase() const;
signals:

private:
    QSqlDatabase m_db;
    QString m_dbName;
    QString m_connectionName;

    bool connect();
};

#endif // DATABASE_H
