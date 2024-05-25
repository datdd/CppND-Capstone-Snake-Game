#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player
{
public:
    Player();
    Player(std::string name, int score);
    Player(std::string name, int score, int level, int health);
    ~Player();

    std::string GetName() const;
    void SetName(std::string name);

    int GetScore() const;
    void SetScore(int score);
    
    int GetLevel() const;
    void SetLevel(int level);
    
    int GetHealth() const;
    void SetHealth(int health);

private:
    std::string name;
    int score;
    int level;
    int health;
};

#endif // PLAYER_H