#include "include/home_connector.h"

HomeConnector::HomeConnector(QObject *parent)
    : QObject{parent}
{
    m_session = new Session();
    m_session = nullptr;
    m_session_running = false;
    m_database = new Database(this, "HomeConnector");
    m_database->open();

    m_effect_start.setSource(QUrl::fromLocalFile(":/sounds/start.wav"));
    m_effect_start.setLoopCount(1);
    m_effect_start.setVolume(0.50f);

    m_effect_end.setSource(QUrl::fromLocalFile(":/sounds/end.wav"));
    m_effect_end.setLoopCount(1);
    m_effect_end.setVolume(0.50f);

}

HomeConnector::~HomeConnector()
{
    m_session->deleteLater();
    m_database->close();
    QString connection_Name = m_database->getQSqlDatabase().connectionName();
    delete(m_database);
    QSqlDatabase::removeDatabase(connection_Name);
}

QString HomeConnector::getAllExercises() const
{
    QJsonObject jsonAll;

    DbRows * results = m_database->getAll("exercise", "", "position, id");

    for (int i = 0; i < results->length(); i++) {
        QJsonObject jsonOne {
            {"name", results->at(i).value("name").toString()}
        };
        jsonAll.insert(QString::number(i), jsonOne);
    }

    return HomeConnector::qJsonObjectToQString(jsonAll);
}

QString HomeConnector::getSessionData() const
{
    QJsonObject jsonAll;

    if (!m_session) {
        qDebug() << "Session not loaded in " << Q_FUNC_INFO;
        return "";
    }

    ExerciseList remainingExercises = m_session->getRemainingExercises();
    ExerciseList doneExercises = m_session->getDoneExercises();
    Exercise * currentExercise = const_cast<Exercise*>(m_session->getCurrentExercise());

    QJsonObject jsonDone;
    int i = 0;
    for (auto doneExercise : doneExercises) {
        QJsonObject jsonOne {
            {"id", doneExercise->getId()},
            {"name", doneExercise->getName()},
            {"description", doneExercise->getDescription()},
            {"type", doneExercise->getType()},
            {"amount", doneExercise->getAmount()},
        };
        jsonDone.insert(QString::number(i++), jsonOne);
    }
    jsonAll.insert("done", jsonDone);

    if (currentExercise && currentExercise->getId() > 0) {
        QJsonObject jsonNow {
            {"id", currentExercise->getId()},
            {"name", currentExercise->getName()},
            {"description", currentExercise->getDescription()},
            {"type", currentExercise->getType()},
            {"amount", currentExercise->getAmount()},
        };
        jsonAll.insert("now", jsonNow);
    } else {
        QJsonObject jsonNow {};
        jsonAll.insert("now", jsonNow);
    }

    QJsonObject jsonRemaining;
    i = 0;

    for (auto remainingExercise : remainingExercises) {
        QJsonObject jsonOne {
            {"id", remainingExercise->getId()},
            {"name", remainingExercise->getName()},
            {"description", remainingExercise->getDescription()},
            {"type", remainingExercise->getType()},
            {"amount", remainingExercise->getAmount()},
        };
        jsonRemaining.insert(QString::number(i++), jsonOne);
    }
    jsonAll.insert("remaining", jsonRemaining);

    return HomeConnector::qJsonObjectToQString(jsonAll);
}

Session *HomeConnector::getSession() const
{
    return m_session;
}

void HomeConnector::setSession(Session *newSession)
{
    m_session = newSession;
}

int HomeConnector::countdownTime() const
{
    return m_countdownTime;
}

void HomeConnector::setCountdownTime(int newCountdownTime)
{
    m_countdownTime = newCountdownTime;
}

QString HomeConnector::qJsonObjectToQString(QJsonObject &obj) const
{
    QJsonDocument doc(obj);

    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

void HomeConnector::startTimer(int seconds)
{
    int step = 0;
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < seconds * 1000) {
        if (!m_session_running) {
            break;
        }
        if (m_session_paused) {
            timer.start();
        } else if (timer.elapsed() > step ) {
            emit timerUpdate(seconds - step/1000);
            step += 1000;
        }
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void HomeConnector::sendInitialData()
{
    m_session = new Session();

    QString data = getSessionData();

    emit initialDataSent(data);
}

void HomeConnector::startSession()
{
    Setting* setting = new Setting();
    int setTimerDuration = setting->load("countdown").toInt();
    delete(setting);

    m_session->reset();
    QString data = getSessionData();

    if (m_session->getRemainingExercises().length() > 0) {
        m_session_running = true;
        m_session_paused = false;

        emit sessionStarted(data, setTimerDuration);
    }

}

void HomeConnector::finishSession()
{
    if (!m_session) {
        qDebug() << "Session not loaded in " << Q_FUNC_INFO;
        return;
    }

    m_session_running = false;

    emit sessionFinished(false);
}

void HomeConnector::finishOngoingExercise()
{
    if (!m_session) {
        qDebug() << "Session not loaded in " << Q_FUNC_INFO;
        return;
    }

    Exercise * exercise = const_cast<Exercise *>(m_session->getCurrentExercise());
    exercise->createReport();

    if (!m_session->finishOngoingExercise()) {
        return;
    }



    m_effect_end.play();

    ExerciseList remainingExercises = m_session->getRemainingExercises();
    if (remainingExercises.length() == 0) {
        emit sessionFinished(m_session_running);
    } else {
        QString data = getSessionData();
        emit exerciseFinished(data);
    }
}

void HomeConnector::startNextExercise()
{
    if (!m_session) {
        qDebug() << "Session not loaded in " << Q_FUNC_INFO;
        return;
    }

    m_session->startNextExercise();

    m_effect_start.play();

    QString data = getSessionData();

    emit exerciseStarted(data);

    Exercise * currentExercise = const_cast<Exercise*>(m_session->getCurrentExercise());

    QString type = currentExercise->getType();

    if (type == "sustain") {
        startTimer(currentExercise->getAmount());
        finishOngoingExercise();
        return;
    }
}

void HomeConnector::pauseSession()
{
    m_session_paused = true;

    emit sessionPaused();

}

void HomeConnector::resumeSession()
{
    m_session_paused = false;

    emit sessionResumed();
}


