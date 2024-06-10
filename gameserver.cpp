#include "gameserver.h"

GameServer::GameServer(QObject *parent)
    : QObject{parent}
{

}

void GameServer::init() {
    tcpServer_ = new QTcpServer(this);
    connect(tcpServer_, SIGNAL(newConnection()), this, SLOT(newConnection()));
    if (!tcpServer_->listen(QHostAddress::Any, 33333) && serverStatus_==0) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServer_->errorString());
    } else {
        serverStatus_=1;
        qDebug() << tcpServer_->isListening() << "TCPSocket listen on port";
        qDebug() << "Server started!";
    }
}

void GameServer::gameTick()
{
    for(GameSession* session: game_session_) {
        if(!session->gameOver()) {
            session->gameTick();
        }
    }
}

void GameServer::newConnection() {
    if(serverStatus_==1){
        qDebug() << QString::fromUtf8("Got new connectiom!");
        //            ui->textinfo->append(QString::fromUtf8("У нас новое соединение!"));
        QTcpSocket* clientSocket=tcpServer_->nextPendingConnection();
        int idusersocs=clientSocket->socketDescriptor();
        id_to_socket_[idusersocs]=clientSocket;
        //            connect(id_to_socket_[idusersocs],SIGNAL(readyRead()),this, SLOT(slotReadClient()));
        connect(clientSocket,&QTcpSocket::readyRead,this, &GameServer::slotReadClient);
    }
}

void GameServer::slotReadClient()
{
//    qDebug() << "ready";
    QTcpSocket* clientSocket = (QTcpSocket*)sender();
    int idusersocs=clientSocket->socketDescriptor();

    QDataStream in(clientSocket);
    readMessage(in);
}

void GameServer::readMessage(QDataStream &in) {
    QTcpSocket* clientSocket = (QTcpSocket*)sender();

    MessageType type = parseMessageType(in);
    switch(type) {
    case MessageType::GameLoginRequestType: {
        GameLoginRequest req;
        in >> req;

        bool ok = db_.checkCredentials(req.login, req.password);
        if(ok){
            QDataStream out(clientSocket);
            GameLoginRespone res;
            res.result = true;            
            res.token = getToken(req.login);

            Player* player;
            if(token_to_player_.count(res.token)) {
                player = token_to_player_[res.token];
            } else {
                player = new Player(req.login);
                token_to_player_[res.token] = player;                
                players_.push_back(player);
            }

            out << res;
        } else {
            QDataStream out(clientSocket);
            GameLoginRespone res;
            res.result = false;
            res.token = "";
            out << res;
        }
        qDebug() << "Try to login" << req.login << " " << req.password << ok;
        break;
    }
    case MessageType::NewGameRequestType: {
        NewGameRequest req;
        in >> req;
        Player* player = getPlayer(req.token);
        if(!player) {
            QDataStream out(clientSocket);
            NewGameResponse res;
            res.result = false;
            out << res;
        }

        if(game_session_.empty()) {
            game_session_.append(new GameSession());
            connect(game_session_.last(), &GameSession::isGameOver, [&](const QString& login){

                auto record = db_.getRecordByLogin(login);
                db_.insertOrUpdateLeaderboard(login, ++record);
            });
        }

        player_to_session_.remove(player);
        bool ok = false;
        for(GameSession* session: game_session_) {
            if(session->isGameStarted()) {
                ok = false;
                break;
            }
            if(session->playerCount() < 5) {
                session->AddPlayer(player);
                player_to_session_[player] = session;
                ok = true;
                break;
            }
        }

        if(!ok) {
            game_session_.append(new GameSession());
            connect(game_session_.last(), &GameSession::isGameOver, [&](const QString& login){

                auto record = db_.getRecordByLogin(login);
                db_.insertOrUpdateLeaderboard(login, ++record);
            });
            game_session_.last()->AddPlayer(player);
            player_to_session_[player] = game_session_.last();
        }
        NewGameResponse res;
        QDataStream out(clientSocket);
        res.result = true;
        out << res;
        break;
    }
    case MessageType::ReadyRequestType: {
        ReadyRequest req;
        in >> req;
        auto player = getPlayer(req.token);
        player->setReady(true);
        GameSession* session = player_to_session_[player];
        if(session) {
            session->startTimer(10);
        }

        qDebug() << "Ready to game ";
        break;
    }
    case MessageType::GetCardRequestType: {
        GetCardRequest req;
        in >> req;
        auto player = getPlayer(req.token);
        if(!player) return;
        GameSession* session = player_to_session_[player];
        session->getCard(player);
        break;
    }
    case MessageType::RegistrUserRequestType: {
        RegistrUserRequest req;
        in >> req;
        bool ok = db_.addUser(req.login, req.password);
        QDataStream out(clientSocket);

        RegistrUserResponse res;
        res.result = ok;
        res.login = req.login;
        out << res;
        qDebug() << "Registr new user" << req.login << req.password << ok;
        break;
    }
    case MessageType::GameStateRequestType: {

        GameStateRequest req;
        in >> req;
        auto player = getPlayer(req.token);
        if(!player) {
            return;
        }

        QDataStream out(clientSocket);
        GameStateResponse res;

        res.state.hand = player->getHand();
        GameSession* session = player_to_session_[player];
        if(!session) return;

        res.state.isYourMove = session->isCurrentPlayer(player);
        QList<EnemyPlayer> enemy;
        auto all_players = session->getPlayers();
        auto index = all_players.indexOf(player);

        for(int i = index + 1; i < all_players.size(); ++i) {
            auto* ep = all_players.at(i);
            enemy.append({ep->name(), ep->cardCount()});
        }
        for(int i = 0; i < index; ++i) {
            auto* ep = all_players.at(i);
            enemy.append({ep->name(), ep->cardCount()});
        }

        res.state.players = enemy;
        res.state.table = session->getTable();
        res.state.gameOver = session->gameOver();

        res.state.youWin = session->isWinPlayer(player);

        qDebug() << "GameStateRequestType";
        out << res;
        break;
    }

    case MessageType::MoveRequestType: {
        qDebug() << "MoveRequestType";
        MoveRequest req;
        in >> req;
        auto player = getPlayer(req.token);
        if(!player) {
            return;
        }

        QDataStream out(clientSocket);
//        GameSession* session = player_to_session_[player];
        player->setDiscardedCard(req.card);
        break;
    }
    case MessageType::LeaderboardRequestType: {
        qDebug() << "LeaderboardRequest";
        LeaderboardRequest req;
        in >> req;
        LeaderboardResponse res;
        res.leaderboard =  db_.getLeaderboard();

        QDataStream out(clientSocket);
        out << res;
        break;
    }
    }
}

QString GameServer::getToken(const QString &login) {
    if(login_to_token_.count(login) > 0) {
        return login_to_token_[login];
    } else {
        login_to_token_[login] = generateToken();
        return login_to_token_[login];
    }
}

Player *GameServer::getPlayer(const QString &token) {
    if(token_to_player_.count(token) > 0) {
        return token_to_player_[token];
    } else {
        return nullptr;
    }
}
