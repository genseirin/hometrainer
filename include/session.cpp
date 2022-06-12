#include "session.h"

Session::Session(QObject *parent)
    : QObject{parent}
{

    this->setObjectName("session" + QDateTime::currentDateTime().toString());
    setStartTime(QDateTime::currentDateTime());
    m_doneExercises = {};
    m_remainingExercises = {};

    m_database = new Database(this, "Session");
    m_database->open();

    reset();
}

Session::~Session()
{
    qDeleteAll(m_doneExercises.begin(), m_doneExercises.end());
    qDeleteAll(m_remainingExercises.begin(), m_remainingExercises.end());
    m_doneExercises.clear();
    m_remainingExercises.clear();
    delete(m_currentExercise);

    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

const QDateTime &Session::getStartTime() const
{
    return m_startTime;
}

void Session::setStartTime(const QDateTime &newStartTime)
{
    m_startTime = newStartTime;
}

const ExerciseList &Session::getDoneExercises() const
{
    return m_doneExercises;
}

void Session::setDoneExercises(const ExerciseList &newDoneExercises)
{
    m_doneExercises = newDoneExercises;
}

const ExerciseList &Session::getRemainingExercises() const
{
    return m_remainingExercises;
}

void Session::setRemainingExercises(const ExerciseList &newRemainingExercises)
{
    m_remainingExercises = newRemainingExercises;
}

const Exercise* Session::getCurrentExercise() const
{
    return m_currentExercise;
}

void Session::startNextExercise()
{
    finishOngoingExercise();

    if (m_remainingExercises.length() > 0) {

        m_currentExercise = m_remainingExercises.takeFirst();
    }
}

bool Session::finishOngoingExercise()
{
    if (!m_currentExercise || !m_currentExercise->getId()) {
        qDebug() << "Cannot find current exercise";
        return false;
    }
    m_doneExercises.push_front(m_currentExercise);
    m_currentExercise = {};

    return true;
}

void Session::reset()
{
    load();
    setDoneExercises({});

    m_currentExercise = nullptr;
}

void Session::load()
{
    QList<int> ids = m_database->getIDs("exercise", "active = 1", "position, id");

    ExerciseList remainingExercises = {};

    for (int id : ids) {
        Exercise * exercise = new Exercise(this, m_database, id);
        remainingExercises.append(exercise);
    }

    setRemainingExercises(remainingExercises);
    remainingExercises = getRemainingExercises();
}

void Session::setCurrentExercise(const Exercise &newCurrentExercise)
{
    m_currentExercise = const_cast <Exercise*>(&newCurrentExercise);
}
