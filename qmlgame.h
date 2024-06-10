#ifndef QMLGAME_H
#define QMLGAME_H

#include "domain.h"
#include "gameclient.h"

#include <QAbstractListModel>

#include <QAbstractListModel>
#include <QList>
#include <QPair>

class LeaderboardModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit LeaderboardModel(QObject *parent = nullptr)
        : QAbstractListModel(parent)
    {
    }

    // Роли данных, которые будут отображаться в QML
    enum LeaderboardRoles {
        LoginRole = Qt::UserRole + 1,
        RecordRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override {
        Q_UNUSED(parent)
        return m_leaderboard.count();
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override {
        if (index.row() < 0 || index.row() >= m_leaderboard.count())
            return QVariant();

        const auto &leader = m_leaderboard[index.row()];
        if (role == LoginRole)
            return leader.first;
        else if (role == RecordRole)
            return leader.second;
        return QVariant();
    }

    QHash<int, QByteArray> roleNames() const override {
        QHash<int, QByteArray> roles;
        roles[LoginRole] = "login";
        roles[RecordRole] = "record";
        return roles;
    }
    void setLeadeboard(QList<QPair<QString, int>> leaderboard) {
        beginResetModel();
        m_leaderboard = leaderboard;
        endResetModel();
    }

private:
    QList<QPair<QString, int>> m_leaderboard;
};

class EnemyModel : public QAbstractListModel {
    Q_OBJECT
public:
    EnemyModel() {
    }
    enum Role {
        Name,
        CardCount
    };
public:
    int rowCount(const QModelIndex &parent) const {
        return enemy.size();
    }
    QVariant data(const QModelIndex &index, int role) const {
        if(!index.isValid())
            return {};
        switch(role) {
        case Name: return enemy.at(index.row()).name; break;
        case CardCount: return enemy.at(index.row()).cardCount; break;
        default: return {};
        }
        return {};
    }
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> map;
        map[Name] = "name";
        map[CardCount] = "cardCount";
        return map;
    }
    void setEnemy(const QList<EnemyPlayer>& enemy) {
        beginResetModel();
        this->enemy = enemy;
        endResetModel();
    }
private:
    QList<EnemyPlayer> enemy;
};

class TableModel : public QAbstractListModel {
    Q_OBJECT
public:
    TableModel() {
    }
    enum Role {
        Color,
        Cost
    };
public:
    int rowCount(const QModelIndex &parent) const {
        return table.size() > 0 ? 1 : 0;
    }
    QVariant data(const QModelIndex &index, int role) const {
        if(!index.isValid())
            return {};
        switch(role) {
        case Color: return table.last().color(); break;
        case Cost: return table.last().cost(); break;
        default: return {};
        }
        return {};
    }
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> map;
        map[Color] = "color";
        map[Cost] = "cost";
        return map;
    }
    void setTable(const  QList<Card>& table) {
        beginResetModel();
        this->table = table;
        endResetModel();
    }
private:
    QList<Card> table;
};


class HandModel : public QAbstractListModel {
public:
    HandModel() {
    }
    enum Role {
        Color,
        Cost
    };
public:
    int rowCount(const QModelIndex &parent) const {
        return hand.cards.size();
    }
    QVariant data(const QModelIndex &index, int role) const {
        if(!index.isValid())
            return {};
        switch(role) {
        case Color: return hand.cards.at(index.row()).color(); break;
        case Cost: return hand.cards.at(index.row()).cost(); break;
        default: return {};
        }
        return {};
    }
    QHash<int, QByteArray> roleNames() const {
        QHash<int, QByteArray> map;
        map[Color] = "color";
        map[Cost] = "cost";
        return map;
    }
    void setHand(const Hand& hand) {
        beginResetModel();
        this->hand = hand;
        endResetModel();
    }
private:
    Hand hand;
};



class QmlGame : public QObject
{
    Q_OBJECT
public:
    QmlGame(GameClient* gameClient)
        : client(gameClient) {
        hand = new HandModel();
        enemy = new EnemyModel();
        table = new TableModel();
        leaderboard = new LeaderboardModel();
    }
    Q_INVOKABLE QObject* getHand() {
        return hand;
    }
    Q_INVOKABLE QObject* getEnemy() {
        return enemy;
    }
    Q_INVOKABLE QObject* getTable() {
        return table;
    }
    Q_INVOKABLE QObject* getLeaderboard() {
        return leaderboard;
    }
public slots:
    void onStateChanged() {
        GameState st = client->gameState();
        hand->setHand(st.hand);
        enemy->setEnemy(st.players);
        table->setTable(st.table);
    }
    void onGotLeaderBoard(QList<QPair<QString, int>> leaderboard) {
        this->leaderboard->setLeadeboard(leaderboard);
    }
private:
    GameClient* client;
    HandModel* hand;
    EnemyModel* enemy;
    TableModel* table;
    LeaderboardModel* leaderboard;
};

#endif // QMLGAME_H
