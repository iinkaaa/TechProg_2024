#ifndef GAMESESSION_H
#define GAMESESSION_H

#include "domain.h"
#include "player.h"

#include <QObject>
#include <unordered_map>
#include "domain.h"
#include <QDebug>
class GameSession : public QObject
{
    Q_OBJECT
public:
    explicit GameSession(QObject *parent = nullptr);
    void AddPlayer(Player* player);
    void init();
    bool isPlayersReady();
    void gameTick();
    int playerCount() const;
    void nextCurrentPlayer();
    bool getCard(Player* player);
    bool isCurrentPlayer(Player* player);
    bool isWinPlayer(Player* player);
    bool isGameStarted() {
        return gameStarted_;
    }
    QList<Player*> getPlayers() const;
    QList<Card> getTable() const;
    bool gameOver() const {
        return game_over_;
    }
    Player* getWinPlayer() {
        return winPlayer_;
    }
protected:
    void timerEvent(QTimerEvent *event);
signals:
    void isGameOver(QString login);
private:
    bool thisGameOver();
    const int START_CARD_COUNT = 4;
private:
    QList<Player*> player_;
    uint player_counter_ = 0;
    Deck deck_;
    Table table_;
    std::unordered_map<int, Player*> uid_to_player_;
    std::unordered_map<int, Hand> uid_to_hands_;
    int current_player_row_ = 0;
    bool game_over_ = false;
    Player* winPlayer_ = nullptr;

    bool gameStarted_ = false;


};

#endif // GAMESESSION_H
