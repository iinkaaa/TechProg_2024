#ifndef GAME_H
#define GAME_H

#include "gamesession.h"

#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    explicit Game(QObject *parent = nullptr);
    void gameTick(int deltaTime);
    void addGameSession();
    QList<GameSession>& getGameSession();

signals:
private:
    QList<GameSession> session_;
};

#endif // GAME_H
