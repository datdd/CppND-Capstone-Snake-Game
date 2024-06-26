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
        Game game(kGridWidth, kGridHeight);
        game.EnterPlayer(renderer);
        game.Run(controller, renderer, kMsPerFrame);
        game.WriteToScoreboard();
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