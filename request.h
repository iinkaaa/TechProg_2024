#ifndef REQUEST_H
#define REQUEST_H

#include "domain.h"
#include "player.h"
enum MessageType {
    GameLoginRequestType,
    GameLoginResponseType,
    NewGameRequestType,
    NewGameResponseType,
    GameStateRequestType,
    GameStateResponseType,
    MoveRequestType,
    MoveResponseType,
    RegistrUserRequestType,
    RegistrUserResponseType,
    ReadyRequestType,
    GetCardRequestType,
    GetCardResponseType,
    LeaderboardRequestType,
    LeaderboardResponseType
};
inline QDataStream &operator<<(QDataStream &out, const MessageType &type) {
    out << static_cast<int>(type);
    return out;
}

inline QDataStream &operator>>(QDataStream &in, MessageType &type) {
    int value;
    in >> value;
    type = static_cast<MessageType>(value);
    return in;
}
struct Message {
//    virtual MessageType type() const = 0;
//    MessageType type = MoveResponse;
    MessageType type;
};

struct ReadyRequest: public Message {
    ReadyRequest() {
        type = ReadyRequestType;
    }
    friend QDataStream &operator<<(QDataStream &out, const ReadyRequest &req) {
        out << req.type << req.token;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, ReadyRequest &response) {
        in >> response.token;
        return in;
    }
    QString token;
};

struct RegistrUserRequest: public Message {
    RegistrUserRequest() {
        type = RegistrUserRequestType;
    }
    friend QDataStream &operator<<(QDataStream &out, const RegistrUserRequest &req) {
        out << req.type << req.login << req.password;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, RegistrUserRequest &response) {
        in >> response.login >> response.password;
        return in;
    }
    QString login;
    QString password;
};

struct RegistrUserResponse: public Message {
    RegistrUserResponse() {
        type = RegistrUserResponseType;
    }
    friend QDataStream &operator<<(QDataStream &out, const RegistrUserResponse &req) {
        out << req.type << req.login << req.result;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, RegistrUserResponse &response) {
        in >> response.login >> response.result;
        return in;
    }
    QString login;
    bool result;
};

struct GetCardRequest: public Message {
    GetCardRequest() {
        type = GetCardRequestType;
    }
    friend QDataStream &operator<<(QDataStream &out, const GetCardRequest &req) {
        out << req.type << req.token;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GetCardRequest &response) {
        in >> response.token;
        return in;
    }
    QString token;

};

struct GetCardResponse: public Message {
    GetCardResponse() {
        type = GetCardResponseType;
    }
    friend QDataStream &operator<<(QDataStream &out, const GetCardResponse &req) {
        out << req.type << req.card;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GetCardResponse &response) {
        in >> response.card;
        return in;
    }

    Card card;
};

struct GameLoginRequest : public Message {
    GameLoginRequest() {
        type = MessageType::GameLoginRequestType;
    }

    friend QDataStream &operator<<(QDataStream &out, const GameLoginRequest &req) {
        out << req.type << req.login << req.password;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GameLoginRequest &response) {
        in >> response.login >> response.password;
        return in;
    }

    QString login;
    QString password;
};

struct GameLoginRespone : public Message {
    GameLoginRespone() {
        type = MessageType::GameLoginResponseType;
    }
    friend QDataStream &operator<<(QDataStream &out, const GameLoginRespone &res) {
        out << res.type << res.result << res.token;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GameLoginRespone &res) {
        in >> res.result >> res.token;
        return in;
    }
    bool result = 0;
    QString token;
};

struct NewGameRequest : public Message {
    NewGameRequest() {
        type = MessageType::NewGameRequestType;
    }
    friend QDataStream &operator<<(QDataStream &out, const NewGameRequest &res) {
        out << res.type << res.token;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, NewGameRequest &res) {
        in >> res.token;
        return in;
    }

    QString token;
};


struct NewGameResponse : public Message {
    NewGameResponse() {
        type = MessageType::NewGameResponseType;
    }

    friend QDataStream &operator<<(QDataStream &out, const NewGameResponse &res) {
        out << res.type << res.result;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, NewGameResponse &res) {
        in >> res.result;
        return in;
    }
    bool result;
};



struct GameStateRequest : public Message {
    GameStateRequest() {
        type = MessageType::GameStateRequestType;
    }
    friend QDataStream &operator<<(QDataStream &out, const GameStateRequest &res) {
        out << res.type << res.token;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GameStateRequest &response) {
        in >> response.token;
        return in;
    }
    QString token;
};


struct GameStateResponse : public Message {
    GameStateResponse() {
        type = MessageType::GameStateResponseType;
    }
    friend QDataStream &operator<<(QDataStream &out, const GameStateResponse &response) {
        out << response.type << response.state;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, GameStateResponse &response) {
        in >> response.state;
        return in;
    }
    GameState state;
};


struct MoveRequest : public Message {
    MoveRequest() {
        type = MoveRequestType;
    }

    friend QDataStream &operator<<(QDataStream &out, const MoveRequest &request) {
        out << request.type << request.token << request.card;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, MoveRequest &request) {
        in >> request.token >> request.card;
        return in;
    }
    Card card;
    QString token;
};

struct MoveResponse : public Message {
    MoveResponse() {
        type = MoveResponseType;
    }

    bool success;

    friend QDataStream &operator<<(QDataStream &out, const MoveResponse &response) {
        out << response.type << response.success;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, MoveResponse &response) {
        in >> response.success;
        return in;
    }
};

inline MessageType parseMessageType(QDataStream& in) {
    MessageType type;
    in >> type;
    return type;
}


struct LeaderboardRequest : public Message {
    LeaderboardRequest() {
        type = LeaderboardRequestType;
    }

    friend QDataStream &operator<<(QDataStream &out, const LeaderboardRequest &request) {
        out << request.type;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, LeaderboardRequest &request) {
        return in;
    }
};

struct LeaderboardResponse : public Message {
    LeaderboardResponse() {
        type = LeaderboardResponseType;
    }


    friend QDataStream &operator<<(QDataStream &out, const LeaderboardResponse &response) {
        out << response.type << response.leaderboard;
        return out;
    }

    friend QDataStream &operator>>(QDataStream &in, LeaderboardResponse &response) {
        in >> response.leaderboard;
        return in;
    }
    QList<QPair<QString, int>> leaderboard;
};

#endif // REQUEST_H
