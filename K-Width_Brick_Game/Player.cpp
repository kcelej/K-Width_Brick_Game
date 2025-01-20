#include "Player.h"

Player::Player(const QString& ip, int id) : ipAddress(ip), playerId(id) {}

Player::~Player() {
    // Nic do specjalnego czyszczenia
}

QString Player::getIpAddress() const { return ipAddress; }
int Player::getId() const { return playerId; }

void Player::setIpAddress(const QString& ip) { ipAddress = ip; }
void Player::setId(int id) { playerId = id; }

Player& Player::operator=(const Player& rv) {
    this->ipAddress = rv.ipAddress;
    this->playerId = rv.playerId;
    return *this;
}
Player::Player(const Player& toCopy) {
    this->ipAddress = toCopy.ipAddress;
    this->playerId = toCopy.playerId;
}