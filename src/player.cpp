#include "player.h"

Player::Player() {}
Player::Player(std::string name, int score)
    : name(std::move(name)), score(score) {}
Player::Player(std::string name, int level, int score, int health)
    : name(std::move(name)), level(level), score(score), health(health) {}

Player::~Player() = default;

std::string Player::GetName() const {
    return name;
}
void Player::SetName(std::string name) {
    this->name = name;
}

int Player::GetScore() const {
    return score;
}
void Player::SetScore(int score) {
    this->score = score;
}

int Player::GetLevel() const {
    return level;
}
void Player::SetLevel(int level) {
    this->level = level;
}

int Player::GetHealth() const {
    return health;
}
void Player::SetHealth(int health) {
    this->health = health;
}