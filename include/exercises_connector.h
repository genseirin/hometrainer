#ifndef EXERCISECONNECTOR_H
#define EXERCISECONNECTOR_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>
#include "include/exercise.h"
#include "include/database.h"

class ExercisesConnector : public QObject
{
    Q_OBJECT
public:
    explicit ExercisesConnector(QObject *parent = nullptr);
    ~ExercisesConnector();

private:
    Database *m_database;

    QString qJsonObjectToQString(QJsonObject &obj) const;

signals:
    void dataSent(QString data);
    void dataSaved();

public slots:
    void getExercises();
    void updateExercises(QString data);
    void deleteExercise(int id);
    void createExercise();
};

#endif // EXERCISECONNECTOR_H
