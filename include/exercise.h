#ifndef PLAN_H
#define PLAN_H

#include <QObject>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDateTime>
#include "include/database.h"
#include "include/report.h"

class Exercise : public QObject
{
    Q_OBJECT
public:
    explicit Exercise(QObject *parent = nullptr, Database *database = nullptr, int id = 0);
    ~Exercise();

    bool load();
    bool save();
    int getId() const;
    void setId(int newId);

    bool getActive() const;
    void setActive(bool newActive);

    const QString &getName() const;
    void setName(const QString &newName);

    const QString &getDescription() const;
    void setDescription(const QString &newDescription);

    const QDateTime &getModified() const;
    void setModified(const QDateTime &newModified);

    quint16 getAmount() const;
    void setAmount(quint16 newAmount);

    QString getType() const;
    void setType(QString newType);

    quint16 getPosition() const;
    void setPosition(quint16 newPosition);

    const QDateTime &getLastdone() const;
    void setLastdone(const QDateTime &newLastdone);

    void createReport();
    bool deleteExercise();

signals:

private:
    QSqlQuery m_query;
    Database* m_database;

    int m_id;
    bool m_active;
    quint16 m_position;
    QString m_name;
    QString m_description;
    QDateTime m_modified;
    QDateTime m_lastdone;
    quint16 m_amount;
    QString m_type;

    bool create();
    bool update();
    bool insert();
    bool rowExists();
    int lastId();
    bool execQuery();
};

#endif // PLAN_H
