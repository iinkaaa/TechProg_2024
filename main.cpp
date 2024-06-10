#include <QApplication>
#include <QCoreApplication>
#include <QPushButton>
#include <QQmlApplicationEngine>
#include "domain.h"
#include "gameclient.h"
#include "gameserver.h"
#include "qmlgame.h"
#include <QTimer>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QQmlApplicationEngine engine;
        if(argc < 1) return -1;
        try {
            if(*argv[1] == '1') {
                qDebug() << "server";
                GameServer* server = new GameServer;
                server->init();
            } else {
                qDebug() << "client";
                GameClient* client = new GameClient;
                client->connectToServer("127.0.0.1", 33333);
                QmlGame* qmlGame = new QmlGame(client);
                QObject::connect(client, &GameClient::gameStateChanged, qmlGame, &QmlGame::onStateChanged);
                QObject::connect(client, &GameClient::gotLeaderBoard, qmlGame, &QmlGame::onGotLeaderBoard);

                engine.rootContext()->setContextProperty("gameClient", client);
                engine.rootContext()->setContextProperty("qmlGame", qmlGame);
                qmlRegisterType<Card>("Game", 1, 0, "Card");
                 qmlRegisterType<EnemyPlayer>("Game", 1, 0, "Enemy");
                engine.load(QUrl(QStringLiteral("qrc:/App.qml")));
                if (engine.rootObjects().isEmpty())
                    return -1;

                 // QTimer::singleShot(100,[&](){
                 //     client->registr("1", "1");

                 // });
//                 QTimer::singleShot(100,[&](){
//                     client->registr("2", "2");

//                 });
//                 QTimer::singleShot(100,[&](){
//                     client->registr("3", "3");

//                 });
            }
        }
        catch(std::exception& ex) {
            qDebug() << ex.what();
        }

    qDebug() << generateToken();
    return a.exec();
}
