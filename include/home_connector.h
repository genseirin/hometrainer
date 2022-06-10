#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QElapsedTimer>
#include "include/exercise.h"
#include "include/setting.h"
#include "include/database.h"
#include "include/session.h"

typedef QMap<QString, QVariant> DbRow;
typedef QList<DbRow> DbRows;
typedef QList<Exercise *> ExerciseList;

class HomeConnector : public QObject
{
    Q_OBJECT

public:
    explicit HomeConnector(QObject *parent = nullptr);
    ~HomeConnector();

    QString getAllExercises() const;
    QString getSessionData() const;

    int getOne() {return 10;}
    void setOne(int one) {}

    Session *getSession() const;
    void setSession(Session *newSession);
    
    int countdownTime() const;
    void setCountdownTime(int newCountdownTime);

private:
    Database* m_database;
    Session *m_session;
    int m_countdownTime;
    bool m_session_running;
    bool m_session_paused;

    QString qJsonObjectToQString(QJsonObject &obj) const;

    void startTimer(int seconds);

signals:
    void exerciseStarted(QString data);
    void exerciseFinished(QString data); // next exercise will follow
    void timerUpdate(int seconds);
    void sessionStarted(QString data, int setTimerDuration);
    void sessionFinished(bool regular);
    void sessionPaused();
    void sessionResumed();
    void initialDataSent(QString data);

public slots:
    void sendInitialData();
    void startSession();
    void finishSession();
    void finishOngoingExercise();
    void startNextExercise();
    void pauseSession();
    void resumeSession();

};

#endif // CONNECTOR_H
