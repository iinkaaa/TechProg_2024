#ifndef DOMAIN_H
#define DOMAIN_H

#include <QDataStream>
#include <QQueue>
#include <QVector>
#include <algorithm>

enum Color {
    RED,
    BLUE,
    YELLOW,
    GREEN
};

inline QDataStream &operator<<(QDataStream &out, const Color &color) {
    out << static_cast<int>(color);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, Color &color) {
    int value;
    in >> value;
    color = static_cast<Color>(value);
    return in;
}

class Card  :public QObject{
    Q_OBJECT
public:
    Card(){}
    Card(const Card& other) {
        this->color_ = other.color_;
        this->cost_ = other.cost_;
    }
    Card(Card&& other) {
        this->color_ = other.color_;
        this->cost_ = other.cost_;
    }

    Card(int cost, Color color)
        : cost_{cost}, color_{color} {

    }

    Card& operator=(const Card& state){
        cost_ = state.cost_;
        color_ = state.color_;
        return *this;
    }
    Card& operator=(Card&& state){
        cost_ = state.cost_;
        color_ = state.color_;
        return *this;
    }

    bool operator==(const Card& other) {
        return cost_ == other.cost_ && color_ == other.color_;
    }
    int cost() const {
        return cost_;
    }

    int color() const {
        return color_;
    }
    friend QDataStream &operator<<(QDataStream &out, const Card &card) {
        out << card.cost_ << card.color_;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Card &card) {
        in >> card.cost_ >> card.color_;
        return in;
    }
    Q_PROPERTY(int color READ color WRITE setColor)
    Q_PROPERTY(int cost READ cost WRITE setCost)
    void setColor(int color) {
        color_ = static_cast<Color>(color);
    }
    void setCost(int cost) {
        cost_ = cost;
    }
private:
    int cost_;
    Color color_;
};
// cardCount = (4*10) * 2 = 80

class Deck {
public:
    void generate() {
        QVector<Card> cards;
        for(int count = 0; count < 3; ++count) {
            for(int cost: cadsCost_) {
                for(int colorIndex = 0; colorIndex < 4; ++colorIndex) {
                    Color color = static_cast<Color>(colorIndex);
                    cards.append(Card(cost, color));
                }
            }
        }
        std::random_shuffle(cards.begin(), cards.end());
        for(Card& card: cards) {
            cards_.append(card);
        }
    }
    Card getNext() {
        if(cards_.empty()) {
            generate();
        }
        return cards_.dequeue();
    }
private:
    int CARD_COUNT = 80;
    QVector<int> cadsCost_  = {0,1,2,3,4,5,6,7,8,9};
    QQueue<Card> cards_;
};

struct Hand {
    Hand() {}
    Hand(const Hand& other) {
        cards = other.cards;
    }
    Hand& operator=(const Hand& other) {
        cards = other.cards;
        return *this;
    }
    bool operator==(const Hand& other) {
        return cards == other.cards;
    }
    friend QDataStream &operator<<(QDataStream &out, const Hand &hand) {
        out << hand.cards;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, Hand &hand) {
        in >> hand.cards;
        return in;
    }
    QList<Card> cards;
};

struct Table {
    Card top() {
        return cards.last();
    }
    void operator=(const QList<Card>& cards) {
        this->cards = cards;
    }
    QList<Card> cards;
};


class EnemyPlayer : public QObject {
    Q_OBJECT
public:
    EnemyPlayer() {}
    EnemyPlayer(QString name, int cardCount) {
        this->cardCount = cardCount;
        this->name = name;
    }
    EnemyPlayer(const EnemyPlayer& other) {
        name = other.name;
        cardCount = other.cardCount;
    }
    EnemyPlayer& operator=(const EnemyPlayer& other) {
        name = other.name;
        cardCount = other.cardCount;

    }
    bool operator==(const EnemyPlayer& other) {
        return name == other.name && cardCount == other.cardCount;
    }

    Q_PROPERTY(QString name READ getName)
    Q_PROPERTY(int cardCount READ getCardCount)
    QString getName() {
        return name;
    }
    int getCardCount() {
        return cardCount;
    }

    friend QDataStream &operator<<(QDataStream &out, const EnemyPlayer &player) {
        out << player.name << player.cardCount;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, EnemyPlayer &player) {
        in >> player.name >> player.cardCount;
        return in;
    }


    QString name;
    int cardCount;
};

class GameState : public QObject{
    Q_OBJECT
public:
    GameState() {}

    GameState(const GameState& state) {
        hand = state.hand;
        players = state.players;
        table = state.table;
        isYourMove = state.isYourMove;
        gameOver = state.gameOver;
        youWin = state.youWin;
    }

    GameState& operator=(const GameState& state){
        hand = state.hand;
        players = state.players;
        table = state.table;
        isYourMove = state.isYourMove;
        gameOver = state.gameOver;
        youWin = state.youWin;
        return *this;
    }

    bool operator==(const GameState& state){
        return hand == state.hand &&
        players == state.players &&
        table == state.table &&
        isYourMove == state.isYourMove  &&
        gameOver == state.gameOver&&
        youWin == state.youWin;

    }
    bool operator!=(const GameState& state){
        return !(*this == state);
    }

    Q_PROPERTY(Hand hand READ getHand)


    Hand getHand() {
        return hand;
    }


    friend QDataStream &operator<<(QDataStream &out, const GameState &state) {
        out << state.hand << state.players << state.table << state.isYourMove << state.gameOver << state.youWin;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GameState &state) {
        in >> state.hand >> state.players >> state.table >> state.isYourMove >> state.gameOver >> state.youWin;
        return in;
    }

    Hand hand;
    QList<EnemyPlayer> players;
    QList<Card> table;
    bool isYourMove = false;
    bool gameOver = false;
    bool youWin = false;
};


#endif // DOMAIN_H
