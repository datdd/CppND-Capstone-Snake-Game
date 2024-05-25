#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "player.h"

int main()
{
    constexpr std::size_t kFramesPerSecond{60};
    constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
    constexpr std::size_t kGridWidth{32};
    constexpr std::size_t kGridHeight{32};

    try
    {
        Renderer renderer(kGridWidth, kGridHeight);
        Controller controller;
        std::unique_ptr<Player> player = std::make_unique<Player>("player01", 1, 2, 3);
        Game game(kGridWidth, kGridHeight, std::move(player));
        game.Run(controller, renderer, kMsPerFrame);
        std::cout << "Game has terminated successfully!\n";
        std::cout << "Score: " << game.GetScore() << "\n";
        std::cout << "Size: " << game.GetSize() << "\n";
    }
    catch (const std::exception &e)
    {
        std::cout << "Game has terminated with an error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}