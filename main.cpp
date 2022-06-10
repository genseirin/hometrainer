#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "include/database.h"
#include "include/setting.h"
#include "include/exercise.h"
#include "include/report.h"
#include "include/home_connector.h"
#include "include/reports_connector.h"
#include "include/settings_connector.h"
#include "include/exercises_connector.h"

typedef QList<QMap<QString, QVariant>> DbRows;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<HomeConnector>("com.chattymango.home_connector",1,0,"HomeConnector");
    qmlRegisterType<ReportsConnector>("com.chattymango.reports_connector",1,0,"ReportsConnector");
    qmlRegisterType<SettingsConnector>("com.chattymango.settings_connector",1,0,"SettingsConnector");
    qmlRegisterType<ExercisesConnector>("com.chattymango.exercises_connector",1,0,"ExercisesConnector");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    return app.exec();
}
