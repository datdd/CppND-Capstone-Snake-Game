#ifndef GAME_H
#define GAME_H

#include <mutex>
#include <thread>
#include <future>

#include <memory>
#include <random>

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "player.h"
#include "AStar.h"

class Game
{
public:
    Game(std::size_t grid_width, std::size_t grid_height);
    void Init();
    void Run(Controller const &controller, Renderer &renderer, std::size_t target_frame_duration);
    void EnterPlayer(Renderer &renderer);
    int GetScore() const;
    int GetSize() const;
    void WriteToScoreboard();
    void AutoPlay();

private:
    Snake snake;
    SDL_Point food;
    std::unique_ptr<Player> player;
    std::vector<Player> scores;
    AStar astar;

    std::random_device dev;
    std::mt19937 engine;
    std::uniform_int_distribution<int> random_w;
    std::uniform_int_distribution<int> random_h;

    int score{0};
    bool autoPlay = false;

    std::mutex mtx;

    void PlaceFood();
    void Update();
    void ReadScoreboard();
};

#endif