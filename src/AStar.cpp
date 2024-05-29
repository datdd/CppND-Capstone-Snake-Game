#include "AStar.h"
#include "snake.h"

const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

AStar::AStar(int gridWidth, int gridHeight) : gridWidth(gridWidth), gridHeight(gridHeight) {}
AStar::~AStar() {}
std::queue<SDL_Point> AStar::FindPath(int startX, int startY, int goalX, int goalY, const std::vector<Snake> &snakes)
{
    // Create a priority queue to hold nodes to be processed
    std::priority_queue<Node *, std::vector<Node *>, CompareNodes> queue;

    // Create a map to keep track of visited nodes
    std::unordered_map<int, Node *> visited;

    // Create the start node
    Node *start = new Node(startX, startY, 0, Heuristic(startX, startY, goalX, goalY), nullptr);
    queue.push(start);

    // Create the goal node
    Node *goal = new Node(goalX, goalY, 0, 0, nullptr);

    while (!queue.empty())
    {
        Node *current = queue.top();
        queue.pop();

        // If we've reached the goal, construct the path and return it
        if (current->x == goal->x && current->y == goal->y)
        {
            std::vector<Node *> path;
            while (current != nullptr)
            {
                path.push_back(current);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());

            std::queue<SDL_Point> result;
            for (size_t i = 0; i < path.size(); ++i)
            {
                Node *node = path[i];
                result.push({node->x, node->y});
            }

            // Clean up
            while (!queue.empty())
            {
                delete queue.top();
                queue.pop();
            }
            delete goal;

            return result;
        }

        // Mark the current node as visited
        visited[current->x + current->y * gridWidth] = current;

        // Explore neighbors
        // Loop through current node's potential neighbors.
        for (int i = 0; i < 4; i++)
        {
            int x = current->x + delta[i][0];
            int y = current->y + delta[i][1];

            // Check if the neighbor is within bounds and not occupied by any snake
            bool occupied = false;
            for (const Snake &snake : snakes)
            {
                if (snake.SnakeCell(x, y))
                {
                    std::cout << __FUNCTION__ << " - There is snake here" << std::endl;
                    occupied = true;
                    break;
                }
            }
            if (!occupied && x >= 0 && x < gridWidth && y >= 0 && y < gridHeight)
            {
                int g = current->g + 1;
                int h = Heuristic(x, y, goalX, goalY);
                Node *neighbor = new Node(x, y, g, h, current);

                // If the neighbor has not been visited, add it to the queue
                if (visited.find(x + y * gridWidth) == visited.end())
                {
                    queue.push(neighbor);
                }
            }
        }
    }

    // Clean up
    while (!queue.empty())
    {
        delete queue.top();
        queue.pop();
    }
    delete goal;

    return std::queue<SDL_Point>();
}

int AStar::Heuristic(int x, int y, int goalX, int goalY)
{
    // Manhattan distance heuristic
    return std::abs(x - goalX) + std::abs(y - goalY);
}