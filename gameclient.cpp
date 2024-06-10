#include "domain.h"
#include "gameclient.h"
#include "request.h"


GameClient::GameClient(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::connected, this, &GameClient::connected);
    connect(socket, &QTcpSocket::disconnected, this, &GameClient::disconnected);
    connect(socket, &QTcpSocket::readyRead, this, &GameClient::readyRead);
}

void GameClient::connectToServer(const QString &host, quint16 port)
{
    socket->connectToHost(host, port);
}

void GameClient::sendData(const QByteArray &data)
{
//    qDebug() << "write";
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(data);
        socket->flush();
    }
}

void GameClient::connected()
{
    qDebug() << "Connected to server";
}

void GameClient::disconnected()
{
    qDebug() << "Disconnected from server";
}

void GameClient::readyRead()
{
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
        QDataStream in(clientSocket);

    MessageType type = parseMessageType(in);
    switch(type) {
    case MessageType::GameLoginResponseType: {
        GameLoginRespone res;
        in >> res;
        setLogin(res.result);
        token = res.token;
        qDebug() << "login" << isLogin_;
        break;
    }
    case MessageType::NewGameResponseType: {
        NewGameResponse res;
        in >> res;
        isGameStarted_ = res.result;
        qDebug() << "Game started" << isGameStarted_;
        break;
    }
    case MessageType::GameStateResponseType: {
        qDebug() << "GameState response";
        GameStateResponse res;
        in >> res;
        setGameState(res.state);
        if(res.state.gameOver) {
            isGameStarted_ = false;
        }
//        qDebug() << res.state.hand.cards.size();
        break;
    }
    case MessageType::RegistrUserResponseType: {
        RegistrUserResponse res;
        in >> res;
        qDebug() << "registr user " << res.login <<" "<< res.result;
        setRegistr(res.result);
        break;
    }
    case MessageType::GetCardResponseType: {
        GetCardResponse res;
        in >> res;
        break;
    }
    case MessageType::MoveResponseType: {
        break;
    }
    case MessageType::LeaderboardResponseType: {
        qDebug() << "LeaderboardResponseType";
        LeaderboardResponse res;
        in >> res;
        emit gotLeaderBoard(res.leaderboard);
        break;
    }
    default: {
        qDebug() << "Unexpected type";
        break;
    }
    }
}

void GameClient::tryToLogin(const QString &login, const QString &password) {
    qDebug() << "Send login" << login << password;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    GameLoginRequest request;
    request.login = login;
    request.password = password;

    out << request;

    sendData(block);
}

void GameClient::startGame()
{
    qDebug() << "Send start game";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    NewGameRequest request;
    request.token = this->token;
    out << request;

    sendData(block);
}

void GameClient::updateGameState()
{
//    qDebug() << "Update game state";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    GameStateRequest request;
    request.token = token;

    out << request;

    sendData(block);
}

void GameClient::ready()
{
    qDebug() << "Send ready";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    ReadyRequest request;
    request.token = this->token;
    out << request;

    sendData(block);
    timer_id = startTimer(10);
//        updateGameState();
}

void GameClient::leaderboard()
{
    qDebug() << "Send leadervoard";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    LeaderboardRequest request;
    out << request;

    sendData(block);
}

void GameClient::getCard()
{
    qDebug() << "Send get card";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    GetCardRequest request;
    request.token = this->token;
    out << request;

    sendData(block);
}

void GameClient::move(int color, int cost)
{
    qDebug() << "Send move card";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    MoveRequest request;
    request.token = this->token;
    request.card = Card(cost, static_cast<Color>(color));
    out << request;

    sendData(block);
}

void GameClient::timerEvent(QTimerEvent *event)
{
    updateGameState();
}

void GameClient::registr(const QString &login, const QString &password)
{
    qDebug() << "Send registr new user" << login << password;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    RegistrUserRequest request;
    request.login = login;
    request.password = password;
    out << request;


    sendData(block);
}
