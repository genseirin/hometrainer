#include "include/report.h"


Report::Report(QObject *parent, int id): QObject{parent}
{
    m_exerciseId = 0;
    m_id = id;

    m_database = new Database(this, "Report");
    m_database->open();
    m_query = QSqlQuery(m_database->getQSqlDatabase());

    m_query.exec("SELECT EXISTS( SELECT name FROM sqlite_master WHERE type='table' AND name='report')");

    if (!rowExists()) {
        m_query.finish();
        qDebug() << "We need to create the table report";
        m_query.prepare("CREATE TABLE report ("
                        "id	INTEGER UNIQUE,"
                        "time	INTEGER NOT NULL,"
                        "exercise_id	INTEGER NOT NULL,"
                        "PRIMARY KEY(id AUTOINCREMENT),"
                        "FOREIGN KEY('exercise_id') REFERENCES exercise('id')"
                        ")");
        execQuery();
    } else {
        m_query.finish();
    }

    if (0 == id) {
        m_time = QDateTime::currentDateTime();
    } else {
        load();
    }
}

Report::~Report()
{
    m_query.finish();
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

bool Report::load()
{

    m_query.prepare("SELECT * FROM report WHERE id = :id");
    m_query.bindValue(":id", m_id);

    if (execQuery() && m_query.first()) {
        setTime(QDateTime::fromSecsSinceEpoch(m_query.value(1).toInt()));
        setExerciseId(m_query.value(2).toInt());
        m_query.finish();
        return true;
    }
    m_query.finish();
    return false;
}

const QDateTime &Report::getTime() const
{
    return m_time;
}

void Report::setTime(const QDateTime &newTime)
{
    m_time = newTime;
}

int Report::id() const
{
    return m_id;
}

void Report::setId(int newId)
{
    m_id = newId;
}

int Report::getExerciseId() const
{
    return m_exerciseId;
}

void Report::setExerciseId(int newExerciseId)
{
    m_exerciseId = newExerciseId;
}

bool Report::save()
{
    m_query.prepare("SELECT EXISTS( SELECT * FROM report WHERE id = :id )");
    m_query.bindValue(":id", m_id);
    execQuery();

    if (rowExists()) {
        m_query.finish();
        update();
    } else {
        m_query.finish();
        insert();
    }

    return true;
}

bool Report::update()
{
    qDebug() << "updating table report";
    m_query.prepare("UPDATE report SET time = :time, exercise_id = :exercise_id WHERE id = :id");
    m_query.bindValue(":time", m_time.toSecsSinceEpoch());
    m_query.bindValue(":exercise_id", m_exerciseId);
    m_query.bindValue(":id", m_id);
    bool ok = execQuery();
    m_query.finish();

    return ok;
}

bool Report::insert()
{
    qDebug() << "inserting table report";
    m_query.prepare("INSERT INTO report (time, exercise_id) VALUES (:time, :exercise_id)");
    m_query.bindValue(":time", m_time.toSecsSinceEpoch());
    m_query.bindValue(":exercise_id", m_exerciseId);
    bool ok = execQuery();
    if (ok) {
        m_id = lastId();
    }
    m_query.finish();

    return ok;
}

bool Report::rowExists()
{
    if (m_query.first()) {
        return m_query.value(0).toInt() ? true : false;
    } else {
        return false;
    }
}

int Report::lastId() {
    m_query.exec("SELECT LAST_INSERT_ROWID()");
    if (m_query.first()) {
        return m_query.value(0).toInt();
    }
    return -1;
}

bool Report::execQuery() {
    if (!m_query.exec()) {
        qDebug() << "Error: " << m_query.lastError().text() << m_query.lastQuery();
        return false;
    }
    return true;
}
