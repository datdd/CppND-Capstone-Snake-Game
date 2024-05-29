#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>

#include "SDL_rect.h"

class Snake;

class Node
{
public:
    Node(int x, int y, int g, int h, Node *parent) : x(x), y(y), g(g), h(h), f(g + h), parent(parent) {}
    ~Node() {}

    int x, y;
    int g, h, f;
    Node *parent;
};

class CompareNodes
{
public:
    bool operator()(const Node *a, const Node *b)
    {
        return a->f > b->f;
    }
};

class AStar
{
public:
    AStar(int gridWidth, int gridHeight);
    ~AStar();
    std::queue<SDL_Point> FindPath(int startX, int startY, int goalX, int goalY, const std::vector<Snake> &snakes);

private:
    int gridWidth;
    int gridHeight;
    int Heuristic(int x, int y, int goalX, int goalY);
};

#endif // ASTAR_H