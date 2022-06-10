#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include "include/database.h"
//#include "include/exercise.h"

class Report : public QObject
{
    Q_OBJECT
public:
    explicit Report(QObject *parent = nullptr, int id = 0 );
    ~Report();

    bool load();
    bool save();

    const QDateTime &getTime() const;
    void setTime(const QDateTime &newTime);

    int id() const;
    void setId(int newId);

    int getExerciseId() const;
    void setExerciseId(int newExercise_id);

signals:

private:
    QSqlQuery m_query;

    int m_id;
    QDateTime m_time;
    int m_exerciseId;
    Database* m_database;

    bool update();
    bool insert();
    bool rowExists();
    int lastId();
    bool execQuery();
};

#endif // HISTORY_H
