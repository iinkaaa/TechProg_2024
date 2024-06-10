#ifndef GAMECLIENT_H
#define GAMECLIENT_H

#include "gamesession.h"
#include "request.h"

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QAbstractListModel>

class GameClient : public QObject
{
    Q_OBJECT
public:
    explicit GameClient(QObject *parent = nullptr);

    void connectToServer(const QString &host, quint16 port);
    void sendData(const QByteArray &data);
    Q_PROPERTY(bool login READ isLogin WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(bool isRegistr READ isRegistr WRITE setRegistr NOTIFY registrChanged)
    Q_PROPERTY(bool gameOver READ isGameOver NOTIFY gameOverChanged)
    Q_PROPERTY(bool youWin READ isYouWin)
public slots:
    void connected();
    void disconnected();
    void readyRead();

    Q_INVOKABLE void tryToLogin(const QString& login, const QString& password);
    Q_INVOKABLE void startGame();
    Q_INVOKABLE void registr(const QString& login, const QString& password);
    Q_INVOKABLE void ready();
    Q_INVOKABLE void leaderboard();

    Q_INVOKABLE void updateGameState();
    Q_INVOKABLE bool isYourMove() {
        return state.isYourMove;
    }

    bool isGameOver() {
        return state.gameOver;
    }
     bool isYouWin() {
        return state.youWin;
    }
    void getCard();
    void move(int color, int cost);

    bool isLogin() const {
        return isLogin_;
    }
    void setLogin(bool flag) {
        isLogin_ = flag;
        emit loginChanged();
    }

    bool isRegistr() const {
        return isRegistr_;
    }
    void setRegistr(bool flag) {
        isRegistr_ = flag;
        emit registrChanged();
    }
    GameState gameState() {
        return state;
    }
    void setGameState(GameState state) {
        if(this->state != state) {
            this->state = state;
            if(state.gameOver) {
                emit gameOverChanged();
                killTimer(timer_id);
            }

            emit gameStateChanged();
        }
    }

protected:
    void timerEvent(QTimerEvent *event);
signals:
    void loginChanged();
    void registrChanged();
    void gameStateChanged();
    void gameOverChanged();
    void gotLeaderBoard(QList<QPair<QString, int>> leaderboard);
private:
    QTcpSocket *socket;
    bool isLogin_ = false;
    bool isRegistr_ = false;
    bool isGameStarted_ = false;
    GameState state;
    MessageType currentMessage;
    QString token;
    int timer_id;

};

#endif // GAMECLIENT_H
