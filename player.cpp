#include "player.h"

Player::Player(QString login, QObject *parent)
    : QObject{parent}, name_(login)
{

}

void Player::setIsMadeMove(bool isMadeMove)
{
    isMadeMove_ = isMadeMove;
}
