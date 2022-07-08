#include "include/exercise.h"


Exercise::Exercise(QObject *parent, Database *database, int id): QObject{parent}
{
    m_id = id;
    if (database) {
        m_database = database;
    } else {
        m_database = new Database(this, "Exercise");
    }
    m_database->open();
    m_query = QSqlQuery(m_database->getQSqlDatabase());

    m_query.exec("CREATE TABLE IF NOT EXISTS exercise ("
                 "id	INTEGER UNIQUE,"
                 "active	integer NOT NULL,"
                 "position	integer NOT NULL,"
                 "name	varchar(255) NOT NULL,"
                 "description	text,"
                 "modified	integer NOT NULL,"
                 "lastdone	integer,"
                 "amount	integer,"
                 "type	varchar(255),"
                 "PRIMARY KEY(id AUTOINCREMENT)"
                 ")");
    m_query.finish();

    if (-1 == m_id) {
        return;
    }

    if (0 == m_id) {
        create();
    } else {
        load();
    }
}

Exercise::~Exercise()
{
    m_query.finish();
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

int Exercise::getId() const
{
    return m_id;
}

void Exercise::setId(int newId)
{
    m_id = newId;
}

bool Exercise::getActive() const
{
    return m_active;
}

void Exercise::setActive(bool newActive)
{
    m_active = newActive;
}

const QString &Exercise::getName() const
{
    return m_name;
}

void Exercise::setName(const QString &newName)
{
    m_name = newName;
}

const QString &Exercise::getDescription() const
{
    return m_description;
}

void Exercise::setDescription(const QString &newDescription)
{
    m_description = newDescription;
}

const QDateTime &Exercise::getModified() const
{
    return m_modified;
}

void Exercise::setModified(const QDateTime &newModified)
{
    m_modified = newModified;
}

quint16 Exercise::getAmount() const
{
    return m_amount;
}

void Exercise::setAmount(quint16 newAmount)
{
    m_amount = newAmount;
}

QString Exercise::getType() const
{
    return m_type;
}

void Exercise::setType(QString newType)
{
    m_type = newType;
}

quint16 Exercise::getPosition() const
{
    return m_position;
}

void Exercise::setPosition(quint16 newPosition)
{
    m_position = newPosition;
}

const QDateTime &Exercise::getLastdone() const
{
    return m_lastdone;
}

void Exercise::setLastdone(const QDateTime &newLastdone)
{
    m_lastdone = newLastdone;
}

void Exercise::createReport()
{
    Report report = Report();
    report.setExerciseId(m_id);
    report.save();
}

bool Exercise::create()
{
    m_active = true;
    m_name = "new exercise";
    m_description = "";
    m_modified = QDateTime::currentDateTime();
    m_lastdone = QDateTime::fromSecsSinceEpoch(0);
    m_amount = 0;
    m_type = "sustain";
    m_position = 0;
    return true;
}

bool Exercise::load()
{
    m_query.prepare("SELECT * FROM exercise WHERE id = :id");
    m_query.bindValue(":id", m_id);

    if (execQuery() && m_query.first()) {
        m_active = m_query.value(1) == 1;
        m_position = m_query.value(2).toInt();
        m_name = QString::fromUtf8(m_query.value(3).toByteArray());
        m_description = m_query.value(4).toString();
        m_modified = QDateTime::fromSecsSinceEpoch(m_query.value(5).toInt());
        m_lastdone = QDateTime::fromSecsSinceEpoch(m_query.value(6).toInt());
        m_amount = m_query.value(7).toInt();
        m_type = m_query.value(8).toString();
        m_query.finish();
        return true;
    }
    m_query.finish();
    return false;
}

bool Exercise::save()
{
    m_query.prepare("SELECT EXISTS( SELECT * FROM exercise WHERE id = :id )");
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

bool Exercise::update()
{
    m_query.prepare("UPDATE exercise SET "
                    "active = :active, "
                    "position = :position, "
                    "name = :name, "
                    "description = :description, "
                    "modified = :modified, "
                    "lastdone = :lastdone, "
                    "amount = :amount, "
                    "type = :type "
                    "WHERE id = :id");
    m_query.bindValue(":active", (m_active?1:0));
    m_query.bindValue(":position", m_position);
    m_query.bindValue(":name", m_name);
    m_query.bindValue(":description", m_description);
    m_query.bindValue(":modified", m_modified.toSecsSinceEpoch());
    m_query.bindValue(":lastdone", m_lastdone.toSecsSinceEpoch());
    m_query.bindValue(":amount", m_amount);
    m_query.bindValue(":type", m_type);
    m_query.bindValue(":id", m_id);
    bool ok = execQuery();
    m_query.finish();
    return ok;
}

bool Exercise::insert()
{
    m_query.prepare("INSERT INTO exercise (active, position, name, description, modified, lastdone, amount, type)"
                    "VALUES (:active, :position, :name, :description, :modified, :lastdone, :amount, :type)");
    m_query.bindValue(":active", (m_active?1:0));
    m_query.bindValue(":position", m_position);
    m_query.bindValue(":name", m_name);
    m_query.bindValue(":description", m_description);
    m_query.bindValue(":modified", m_modified.toSecsSinceEpoch());
    m_query.bindValue(":lastdone", m_lastdone.toSecsSinceEpoch());
    m_query.bindValue(":amount", m_amount);
    m_query.bindValue(":type", m_type);
    bool ok = execQuery();
    m_query.finish();
    if (ok) {
        m_id = lastId();
    }
    return ok;
}

bool Exercise::deleteExercise()
{
    m_query.prepare("DELETE FROM exercise WHERE id = :id");
    m_query.bindValue(":id", m_id);
    bool ok = execQuery();
    m_query.finish();
    return ok;
}

bool Exercise::rowExists()
{
    if (m_query.first()) {
        return m_query.value(0).toInt() ? true : false;
    } else {
        return false;
    }
}

int Exercise::lastId() {
    m_query.exec("SELECT LAST_INSERT_ROWID()");
    if (m_query.first()) {
        return m_query.value(0).toInt();
    }
    return -1;
}

bool Exercise::execQuery() {
    if (!m_query.exec()) {
        qDebug() << "Error: " << m_query.lastError().text() << m_query.lastQuery();
        return false;
    }
    return true;
}
