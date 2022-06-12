#include "include/setting.h"

Setting::Setting(QObject *parent): QObject{parent}
{
    m_database = new Database(this, "Setting");
    m_database->open();
     m_query = QSqlQuery(m_database->getQSqlDatabase());

    m_query.exec("SELECT EXISTS( SELECT name FROM sqlite_master WHERE type='table' AND name='setting')");


    if (!rowExists()) {
        m_query.finish();
        qDebug() << "We need to create the table setting";
        m_query.exec("CREATE TABLE setting ("
                      "id	INTEGER UNIQUE,"
                      "name	varchar(50) NOT NULL,"
                      "value	varchar(255) NOT NULL,"
                      "time	integer,"
                      "PRIMARY KEY(id AUTOINCREMENT)"
                      ")");
        execQuery();
    } else {
        m_query.finish();
    }

    if (load("countdown") == "") {
        save("countdown", "10");
    }
    if (load("new_session_gap") == "") {
        save("new_session_gap", "20");
    }
}

Setting::~Setting()
{
    m_query.finish();
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

void Setting::save(QString name, QString value)
{
    m_query.prepare("SELECT EXISTS( SELECT * FROM setting WHERE name = :name )");
    m_query.bindValue(":name", name);
    execQuery();

    if (rowExists()) {
        m_query.prepare("UPDATE setting SET value = :value WHERE name = :name");
        m_query.bindValue(":name", name);
        m_query.bindValue(":value", value);
        execQuery();
        m_query.last();
    } else {
        m_query.prepare("INSERT INTO setting (name, value) VALUES (:name, :value)");
        m_query.bindValue(":name", name);
        m_query.bindValue(":value", value);
        execQuery();
    }
    m_query.finish();

}

QString Setting::load(QString name)
{
    m_query.prepare("SELECT value FROM setting WHERE name = :name");
    m_query.bindValue(":name", name);
    execQuery();

    if (m_query.first()) {
        QString value = m_query.value(0).toString();
        m_query.finish();
        return value;
    } else {
        m_query.finish();
        return "";
    }
}

bool Setting::rowExists()
{
    if (m_query.first()) {
        return m_query.value(0).toInt() ? true : false;
    } else {
        return false;
    }
}

bool Setting::execQuery() {
    if (!m_query.exec()) {
        qDebug() << "Error: " << m_query.lastError().text() << m_query.lastQuery();
        return false;
    }
    return true;
}
