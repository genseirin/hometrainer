#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include <QDateTime>
#include "include/database.h"
#include "include/exercise.h"

typedef QMap<QString, QVariant> DbRow;
typedef QList<DbRow> DbRows;
typedef QList<Exercise *> ExerciseList;

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);
    ~Session();

    const QDateTime &getStartTime() const;
    void setStartTime(const QDateTime &newStartTime);

    const ExerciseList &getDoneExercises() const;
    void setDoneExercises(const ExerciseList &newDoneExercises);

    const ExerciseList &getRemainingExercises() const;
    void setRemainingExercises(const ExerciseList &newRemainingExercises);

    const Exercise* getCurrentExercise() const;
    void setCurrentExercise(const Exercise &newCurrentExercise);
    void startNextExercise();
    bool finishOngoingExercise();
    void reset();

private:
    QDateTime m_startTime;
    ExerciseList m_doneExercises;
    ExerciseList m_remainingExercises;
    Exercise *m_currentExercise;
    Database* m_database;

    QString qJsonObjectToQString(QJsonObject &obj) const;
    void load();

signals:

};

#endif // SESSION_H
