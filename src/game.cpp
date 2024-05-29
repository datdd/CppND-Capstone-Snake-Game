#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "SDL.h"
#include "game.h"

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      astar(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1))
{
    Init();
}

void Game::Init()
{
    PlaceFood();
    AutoPlay();
    ReadScoreboard();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration)
{
    Uint32 title_timestamp = SDL_GetTicks();
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    int frame_count = 0;
    bool running = true;

    if (nullptr == player)
    {
        return;
    }

    while (running)
    {
        frame_start = SDL_GetTicks();

        // Input, Update, Render - the main game loop.
        controller.HandleInput(running, snake);
        Update();
        renderer.Render(snake, food, player.get(), scores);

        frame_end = SDL_GetTicks();

        // Keep track of how long each loop through the input/update/render cycle
        // takes.
        frame_count++;
        frame_duration = frame_end - frame_start;

        // After every second, update the window title.
        if (frame_end - title_timestamp >= 1000)
        {
            renderer.UpdateWindowTitle(score, frame_count);
            frame_count = 0;
            title_timestamp = frame_end;
        }

        // If the time for this frame is too small (i.e. frame_duration is
        // smaller than the target ms_per_frame), delay the loop to
        // achieve the correct frame rate.
        if (frame_duration < target_frame_duration)
        {
            SDL_Delay(target_frame_duration - frame_duration);
        }
    }
}

void Game::EnterPlayer(Renderer &renderer)
{
    std::string name = renderer.RenderEnterPlayerWindow();
    if (name != "")
    {
        player = std::make_unique<Player>(name, 0, 0, 3);
    }
    else
    {
        player = nullptr;
    }
}

void Game::PlaceFood()
{
    int x, y;
    while (true)
    {
        x = random_w(engine);
        y = random_w(engine);
        // Check that the location is not occupied by a snake item before placing
        // food.
        if (!snake.SnakeCell(x, y))
        {
            food.x = x;
            food.y = y;
            return;
        }
    }
}

void Game::Update()
{
    if (!snake.alive)
    {
        int playerHealth = player.get()->GetHealth();
        if (playerHealth >= 1)
        {
            player.get()->SetHealth(playerHealth - 1);
        }
        else
        {
            return;
        }
    }
    snake.alive = true;
    snake.Update();

    int new_x = static_cast<int>(snake.head_x);
    int new_y = static_cast<int>(snake.head_y);

    // Check if there's food over here
    if (food.x == new_x && food.y == new_y)
    {
        score++;
        PlaceFood();
        // Grow snake and increase speed.
        snake.GrowBody();
        snake.speed += score % 10 == 0 ? 0.02 : 0;

        player.get()->SetScore(score);
        player.get()->SetLevel(static_cast<int>(score / 10));

        AutoPlay();
    }
}

void Game::AutoPlay()
{
    std::vector<Snake> snakes{snake};
    std::queue<SDL_Point> path = astar.FindPath(snake.head_x, snake.head_y, food.x, food.y, snakes);
    path.pop();
    snake.path = path;
    snake.ChangeDirection(path.front());
    // snake.PrintPath();
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::ReadScoreboard()
{
    std::ifstream file("../data/scoreboard.txt");
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open scoreboard file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string name;
        int score;
        if (std::getline(iss, name, ':') && iss >> score)
        {
            scores.emplace_back(Player(name, score));
        }
    }
    file.close();

    std::sort(scores.begin(), scores.end(), [](const Player &a, const Player &b)
              { return a.GetScore() > b.GetScore(); });
}

void Game::WriteToScoreboard()
{
    if (nullptr == player)
    {
        return;
    }

    std::ofstream file("../data/scoreboard.txt", std::ios_base::app);
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open scoreboard file.\n";
        return;
    }

    file << player->GetName() << " : " << player->GetScore() << "\n";

    file.close();
}