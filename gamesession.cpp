#include "gamesession.h"

GameSession::GameSession(QObject *parent)
    : QObject{parent}
{
    deck_.generate();
}

void GameSession::AddPlayer(Player *player) {
    if(gameStarted_) {
        return;
    }
    player_.append(player);
    uid_to_player_[player_counter_] = player;
    player->clear();
    ++player_counter_;
}

void GameSession::init() {
    int playerCount = player_.size();
    for(int i = playerCount; i < 3; ++i) {
        Bot* bot = new Bot(table_, deck_);
        player_.append(bot);
        uid_to_player_[player_counter_] = bot;

        ++player_counter_;
        bot->setReady(true);
    }
    for(int i = 0; i < START_CARD_COUNT; ++i) {
        for(auto& [uid, player]: uid_to_player_) {
            player->addCard(deck_.getNext());
//            uid_to_hands_[uid].cards.append(deck_.getNext());
        }
    }

}

bool GameSession::isPlayersReady() {
    bool result = true;
    for(Player* player: player_) {
        result&= player->isReady();
    }

    gameStarted_ = result;
    return result;
}

void GameSession::gameTick() {
    if(game_over_) {
        return;
    }
    if(!gameStarted_) {
        if(isPlayersReady()) {
            init();
        }
        return;
    }
    Player* current_player_ = player_.at(current_player_row_);
    if(current_player_->name() == "Bot") {
        Bot* bot = qobject_cast<Bot*>(current_player_);
        bot->move();
    }
    if(current_player_->isMadeMove()) {
        Card playerCard = current_player_->discardedCard();

        if(table_.cards.empty()) {
            table_.cards.append(playerCard);
            current_player_->removeCard(playerCard);
            current_player_->setIsMadeMove(false);
            nextCurrentPlayer();
            game_over_ = thisGameOver();
            return;
        }

        auto isSameColor = [&](const Card& lhs, const Card& rhs){
            return lhs.color() == rhs.color();
        };

        auto isSameCost = [&](const Card& lhs, const Card& rhs){
            return lhs.cost() == rhs.cost();
        };

        auto isBeatCard = [&](const Card& lhs, const Card& rhs){
            return isSameColor(lhs, rhs) || isSameCost(lhs, rhs);
        };


        const Card& topCard = table_.top();
        if(isBeatCard(playerCard, topCard)) {
            table_.cards.append(playerCard);
            current_player_->removeCard(playerCard);
            current_player_->setIsMadeMove(false);
            nextCurrentPlayer();
        } else {
            Q_ASSERT("лучше обработать это на стороне клиента");
        }
        current_player_->setIsMadeMove(false);
        game_over_ = thisGameOver();
    }
}

int GameSession::playerCount() const {
    return player_.size();
}

void GameSession::nextCurrentPlayer() {
    current_player_row_++;
    if(current_player_row_ >= player_.size()) {
        current_player_row_ = 0;
    }
}

bool GameSession::thisGameOver() {
    for(Player* player: player_) {
        if(player->cardCount() == 0) {
            winPlayer_ = player;
            if(player->name() != "Bot") {
                emit isGameOver(player->name());
            }
            return true;
        }
    }
    return false;
}

bool GameSession::getCard(Player *player) {
    if(player_.at(current_player_row_) == player) {
    player->addCard(deck_.getNext());
    }
    return true;
}

bool GameSession::isCurrentPlayer(Player *player) {
    return player = player_.at(current_player_row_);
}

bool GameSession::isWinPlayer(Player *player) {
    return player == winPlayer_;
}

QList<Player*> GameSession::getPlayers() const {
    return player_;
}

QList<Card> GameSession::getTable() const {
    return table_.cards;
}

void GameSession::timerEvent(QTimerEvent *event)
{
//    qDebug() << "Game tick";
    // if(gameStarted_ && !gameOver()) {
    gameTick();
    // }
}
