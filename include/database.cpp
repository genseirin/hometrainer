#include "include/database.h"

Database::Database(QObject *parent,  QString connectionName)
    : QObject{parent}
{
    m_dbName = DATABASE_NAME;
    m_connectionName = connectionName;

    this->connect();
}

Database::~Database()
{
    m_db.close();
}

bool Database::connect()
{
    if (m_connectionName.isEmpty()) {
        m_db = QSqlDatabase::addDatabase("QSQLITE");
    } else {
        int i = 0;
        QStringList connectionNames = QSqlDatabase::connectionNames();
        QString requestedConnectionName = m_connectionName;
        while (connectionNames.contains(m_connectionName)) {
            m_connectionName = requestedConnectionName + "_" + QString::number(++i);
        }
        m_db = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    }

    m_db.setDatabaseName(QCoreApplication::applicationDirPath()
                         + QDir::separator()
                         + m_dbName);

    return true;
}

bool Database::open()
{
    if (m_db.isOpen()) {
        return true;
    }

    if(!m_db.open())
    {
        qDebug() << "Failed to open!";
        qDebug() << m_db.lastError().text();
        return false;
    }
    return true;
}

bool Database::close()
{
    if (m_db.isOpen()) {
        m_db.close();
        return true;
    }
    return false;
}

QList<int> Database::getIDs(QString table, QString where, QString orderby)
{
    QString queryString = "SELECT id FROM " + table;
    queryString += where.isEmpty() ? "" : " WHERE " + where;
    queryString += orderby.isEmpty() ? "" : " ORDER BY " + orderby;

    QSqlQuery query(m_db);
    query.prepare(queryString);

    QList<int> result;
    if (!query.exec()) {
        qDebug() << "Error: " << query.lastError().text() << query.lastQuery();
    } else {
        int j=0;
        while (query.next()) {
            result.append(query.value(0).toInt());
        }
    }

    return result;

}

/*!
 * \brief Get raw data from the database
 * \param table table name
 * \param where
 * \param orderby
 * \return
 */
QList<QMap<QString, QVariant>>* Database::getAll(QString table, QString where, QString orderby)
{
    QList<QMap<QString, QVariant>>* result = new QList<QMap<QString, QVariant>>;
    QString queryString = "SELECT * FROM " + table;

    queryString += where.isEmpty() ? "" : " WHERE " + where;
    queryString += orderby.isEmpty() ? "" : " ORDER BY " + orderby;

    QSqlQuery query(m_db);

    query.prepare(queryString);
    if (!query.exec()) {
        qDebug() << "Error: " << query.lastError().text() << query.lastQuery();
    } else {
        int j=0;
        while (query.next()) {
            QMap<QString, QVariant> row = {};
            QSqlRecord record = query.record();
            for (int i = 0; i < record.count(); i++) {
                row[record.fieldName(i)] = query.value(i);
            }
            result->append(row);
        }
    }

    return result;
}


const QSqlDatabase &Database::getQSqlDatabase() const
{
    return m_db;
}

const QString &Database::getDbName() const
{
    return m_dbName;
}

void Database::setDbName(const QString &newDbName)
{
    m_dbName = newDbName;
    if (m_db.isOpen()) {
        Database::close();
    }
    Database::connect();
}
