#ifndef PLAYER_H
#define PLAYER_H

#include "domain.h"

#include <QObject>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QString login, QObject *parent = nullptr);
    bool isMadeMove() const {
        return isMadeMove_;
    }
    void setDiscardedCard(Card card) {
        discardedCard_ = card;
        isMadeMove_ = true;
    }

    Card discardedCard() {
        return discardedCard_;
    }
    void setReady(bool r) {
        isReady_ = r;
    }
    bool isReady() const {
        return isReady_;
    }
    void removeCard(const Card& card) {
        hand.cards.removeOne(card);
    }
    int cardCount() const {
        return hand.cards.size();
    }
    void addCard(const Card& card) {
        hand.cards.append(card);
    }
    Hand getHand() const {
        return hand;
    }
    QString name() const {
        return name_;
    }
    void setIsMadeMove(bool isMadeMove);
    void clear() {
        hand.cards.clear();
        isMadeMove_ = false;
        isReady_ = false;
    }
signals:
protected:
    // QString login;
    bool isMadeMove_ = false;
    bool isReady_ = false;
    Hand hand;
    QString name_;

    Card discardedCard_;
};


class Bot : public Player {
    Q_OBJECT
public:
    Bot(Table& table, Deck& deck) : Player("Bot"), table(table), deck(deck) {}

    void move() {

        auto isSameColor = [&](const Card& lhs, const Card& rhs){
            return lhs.color() == rhs.color();
        };

        auto isSameCost = [&](const Card& lhs, const Card& rhs){
            return lhs.cost() == rhs.cost();
        };

        auto isBeatCard = [&](const Card& lhs, const Card& rhs){
            return isSameColor(lhs, rhs) || isSameCost(lhs, rhs);
        };

        bool ok = false;
        for(Card card: hand.cards) {
            if( isBeatCard(card, table.top())) {
                setDiscardedCard(card);
                ok = true;
            }
        }

        if(!ok) {
            Card card = deck.getNext();
            while(!isBeatCard(card, table.top())) {

                hand.cards.append(deck.getNext());
                card = deck.getNext();
            }
            setDiscardedCard(card);

        }
        setIsMadeMove(true);
    }
private:
    Table& table;
    Deck& deck;
};
#endif // PLAYER_H
