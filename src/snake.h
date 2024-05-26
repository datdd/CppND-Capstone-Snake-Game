#ifndef SNAKE_H
#define SNAKE_H

#include <iostream>
#include <vector>

#include "SDL.h"

class Snake
{
public:
    enum class Direction
    {
        kUp,
        kDown,
        kLeft,
        kRight
    };

    Snake(int grid_width, int grid_height)
        : grid_width(grid_width),
          grid_height(grid_height),
          head_x(grid_width / 2),
          head_y(grid_height / 2)
    {
        std::cout << "Snake constructor" << std::endl;
    }

    // Destructor
    ~Snake()
    {
        std::cout << "Snake Destructor" << std::endl;
    }

    // Copy constructor
    Snake(const Snake &other) : body(other.body), direction(other.direction)
    {
        std::cout << "Snake copy constructor" << std::endl;
    }

    // Move constructor
    Snake(Snake &&other) : body(std::move(other.body)), direction(std::move(other.direction))
    {
        std::cout << "Snake move constructor" << std::endl;
    }

    // Copy assignment operator
    Snake &operator=(const Snake &other)
    {
        std::cout << "Snake copy assignment operator" << std::endl;
        if (this != &other)
        {
            body = other.body;
            direction = other.direction;
        }
        return *this;
    }

    // Move assignment operator
    Snake &operator=(Snake &&other)
    {
        std::cout << "Snake move assignment operator" << std::endl;
        if (this != &other)
        {
            body = std::move(other.body);
            direction = std::move(other.direction);
        }
        return *this;
    }

    void Update();

    void GrowBody();
    bool SnakeCell(int x, int y);

    Direction direction = Direction::kUp;

    float speed{0.1f};
    int size{1};
    bool alive{true};
    float head_x;
    float head_y;
    std::vector<SDL_Point> body;

private:
    void UpdateHead();
    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

    bool growing{false};
    int grid_width;
    int grid_height;
};

#endif