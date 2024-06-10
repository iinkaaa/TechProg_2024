#ifndef GAMESERVER_H
#define GAMESERVER_H

#include "DataBase.h"
#include "domain.h"
#include "request.h"
#include <QObject>
#include <QTcpServer>
#include <QDebug>
#include <QTcpSocket>
#include <QDateTime>
#include <QCryptographicHash>
#include "game.h"

inline QString generateToken(int length = 32) {
    QString token;

    // Создаем случайную строку
    const QString possibleCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < length; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        token.append(nextChar);
    }

    // Добавляем временную метку для уникальности
    token.append(QDateTime::currentDateTimeUtc().toString("yyyyMMddhhmmss"));

    // Хешируем токен для увеличения безопасности
    QByteArray hashedToken = QCryptographicHash::hash(token.toUtf8(), QCryptographicHash::Sha256);
    QString finalToken = QString(hashedToken.toHex());

    return finalToken;
}

class GameServer : public QObject
{
    Q_OBJECT
public:
    explicit GameServer(QObject *parent = nullptr);

    void init();
    void gameTick();
signals:


public slots:
    void newConnection();
    void slotReadClient();
    void readMessage(QDataStream& in);

private:
    QString getToken(const QString& login);
    Player* getPlayer(const QString& token);
    QTcpServer* tcpServer_;
    int serverStatus_;
    QMap<int, QTcpSocket*> id_to_socket_;
    DatabaseManager db_;
    QMap<QString, Player*> token_to_player_;
    QMap<QString, QString> login_to_token_;
    QMap<Player*, GameSession*> player_to_session_;
    Game game;
    QList<GameSession*> game_session_;
    QList<Player*> players_;

};

#endif // GAMESERVER_H
