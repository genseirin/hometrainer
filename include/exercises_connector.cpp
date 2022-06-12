#include "exercises_connector.h"

ExercisesConnector::ExercisesConnector(QObject *parent)
    : QObject{parent}
{
    m_database = new Database(this, "ExercisesConnector");
    m_database->open();

    Exercise exercise = Exercise(this, nullptr, -1); // create DB tables if necessary

}

ExercisesConnector::~ExercisesConnector()
{
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

void ExercisesConnector::getExercises()
{
    QSqlQuery query(m_database->getQSqlDatabase());

    query.prepare("SELECT * FROM exercise ORDER BY position ASC");

    if (!query.exec()) {
        qDebug() << "Error: " << query.lastError().text() << query.lastQuery();
    } else {
        QJsonObject jsonAll;

        int position = 1;
        while (query.next()) {
            QJsonObject jsonOne {
                {"id", query.value(0).toInt()},
                {"active", query.value(1).toInt()},
                {"position", position},
                {"name", query.value(3).toString()},
                {"description", query.value(4).toString()},
                {"type", query.value(7).toString()},
                {"amount", query.value(8).toInt()},
            };
            jsonAll.insert(QString::number(position++), jsonOne);
        }

        emit dataSent(ExercisesConnector::qJsonObjectToQString(jsonAll));
    }

}

void ExercisesConnector::updateExercises(QString data)
{
    QJsonDocument decoded = QJsonDocument::fromJson(data.toUtf8());

    if (!decoded.isNull()) {
        QJsonArray array = decoded.array();
        for (const QJsonValue &item : array) {
            int id = item.toObject().value("id").toInt();
            Exercise exercise = Exercise(this, nullptr, id);
            exercise.setPosition(item.toObject().value("position").toInt());
            exercise.setActive(item.toObject().value("active")==1);
            exercise.setName(item.toObject().value("name").toString());
            exercise.setDescription(item.toObject().value("description").toString());
            exercise.setModified(QDateTime::currentDateTime());
            exercise.setType(item.toObject().value("type").toString());
            exercise.setAmount(item.toObject().value("amount").toInt());
            exercise.save();

            getExercises();
        }
    }
}

void ExercisesConnector::deleteExercise(int id)
{
    Exercise exercise = Exercise(this, nullptr, id);
    exercise.deleteExercise();

    getExercises();
}

void ExercisesConnector::createExercise()
{
    Exercise exercise = Exercise(this, nullptr, 0);

    exercise.setPosition(0);
    exercise.setActive(true);
    exercise.setName("Enter a name");
    exercise.setDescription("");
    exercise.setLastdone(QDateTime::fromSecsSinceEpoch(0));
    exercise.setModified(QDateTime::currentDateTime());
    exercise.setType(0);
    exercise.setAmount(10);
    exercise.save();

    getExercises();
}

QString ExercisesConnector::qJsonObjectToQString(QJsonObject &obj) const
{
    QJsonDocument doc(obj);

    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
