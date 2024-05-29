#include "snake.h"
#include <cmath>
#include <iostream>

void Snake::Update()
{
    SDL_Point prev_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)}; // We first capture the head's cell before updating.
    UpdateHead();
    SDL_Point current_cell{
        static_cast<int>(head_x),
        static_cast<int>(head_y)}; // Capture the head's cell after updating.

    // Update all of the body vector items if the snake head has moved to a new
    // cell.
    if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y)
    {
        UpdateBody(current_cell, prev_cell);

        if (!path.empty())
        {
            path.pop();
            ChangeDirection(path.front());
        }
    }
}

void Snake::ChangeDirection(SDL_Point next_cell)
{
    SDL_Point current_cell{static_cast<int>(head_x), static_cast<int>(head_y)};
    
    int dx = next_cell.x - current_cell.x;
    int dy = next_cell.y - current_cell.y;
    
    if (dx == -1 && dy == 0)
    {
        direction = Direction::kLeft;
    }
    else if (dx == 0 && dy == -1)
    {
        direction = Direction::kUp;
    }
    else if (dx == 1 && dy == 0)
    {
        direction = Direction::kRight;
    }
    else if (dx == 0 && dy == 1)
    {
        direction = Direction::kDown;
    }
}

void Snake::UpdateHead()
{
    switch (direction)
    {
    case Direction::kUp:
        head_y -= speed;
        break;

    case Direction::kDown:
        head_y += speed;
        break;

    case Direction::kLeft:
        head_x -= speed;
        break;

    case Direction::kRight:
        head_x += speed;
        break;
    }

    // Wrap the Snake around to the beginning if going off of the screen.
    head_x = fmod(head_x + grid_width, grid_width);
    head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell)
{
    // Add previous head location to vector
    body.push_back(prev_head_cell);

    if (!growing)
    {
        // Remove the tail from the vector.
        body.erase(body.begin());
    }
    else
    {
        growing = false;
        size++;
    }

    // Check if the snake has died.
    for (auto const &item : body)
    {
        if (current_head_cell.x == item.x && current_head_cell.y == item.y)
        {
            alive = false;
        }
    }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) const
{
    if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y))
    {
        return true;
    }
    for (auto const &item : body)
    {
        if (x == item.x && y == item.y)
        {
            return true;
        }
    }
    return false;
}

// For debug purpose
void Snake::PrintPath()
{
    char board[grid_height][grid_width];
    
    for (int i = 0; i < grid_height; i++)
    {
        for (int j = 0; j < grid_width; j++)
        {
            if (SnakeCell(j, i))
            {
                board[i][j] = '0';
            }
            else
            {
                board[i][j] = '_';
            }
        }
    }

    if (!path.empty())
    {
        while (!path.empty())
        {
            SDL_Point point = path.front();
            board[point.y][point.x] = 'x';
            path.pop();
        }

        for (int i = 0; i < grid_height; i++)
        {
            for (int j = 0; j < grid_width; j++)
            {
                std::cout << board[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
}