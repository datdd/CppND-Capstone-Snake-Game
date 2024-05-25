#include "renderer.h"
#include <iostream>
#include <string>

Renderer::Renderer(const std::size_t grid_width, const std::size_t grid_height)
    : grid_width(grid_width), grid_height(grid_height)
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        throw std::runtime_error("SDL could not initialize.");
    }

    if (TTF_Init() < 0)
    {
        std::cerr << "TTF could not initialize.\n";
        std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
        throw std::runtime_error("SDL could not initialize.");
    }

    // Create Window
    std::size_t screen_width = playerInfoPanel.w + playBoardPanel.w + 30;
    std::size_t screen_height = playBoardPanel.h + 20;
    sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, screen_width,
                                  screen_height, SDL_WINDOW_SHOWN);

    if (nullptr == sdl_window)
    {
        std::cerr << "Window could not be created.\n";
        std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
        throw std::runtime_error("Window could not be created.");
    }

    // Create renderer
    sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED);
    if (nullptr == sdl_renderer)
    {
        std::cerr << "Renderer could not be created.\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
        throw std::runtime_error("Renderer could not be created.");
    }

    // Create font
    font = TTF_OpenFont("../font/lazy.ttf", 16);
    if (nullptr == font)
    {
        std::cerr << "Failed to load font!\n";
        std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
        throw std::runtime_error("Failed to load font!");
    }
}

Renderer::~Renderer()
{
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}

void Renderer::Render(Snake const snake, SDL_Point const &food, Player *player, std::vector<Player> const &players)
{
    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
    // Render Player Info panel
    RenderPlayerInfo(player);
    // Render Score Table panel
    RenderScoreTable(players);

    RenderPlayBoard();
    // Render food
    RenderFood(food);
    // Render snake's body
    RenderSnakeBody(snake);
    // Update Screen
    SDL_RenderPresent(sdl_renderer);
}

void Renderer::RenderPlayerInfo(Player *player)
{
    RenderPanel(sdl_renderer, playerInfoPanel, {220, 220, 220, 255});
    RenderBorder(sdl_renderer, playerInfoPanel, BORDER_COLOR);
    RenderText(sdl_renderer, font, "Player Info", playerInfoPanel.x + 10, playerInfoPanel.y + 10, TEXT_COLOR);
    RenderText(sdl_renderer, font, "Name: " + player->GetName(), playerInfoPanel.x + 10, playerInfoPanel.y + 40, TEXT_COLOR);
    RenderText(sdl_renderer, font, "Level: " + std::to_string(player->GetLevel()), playerInfoPanel.x + 10, playerInfoPanel.y + 70, TEXT_COLOR);
    RenderText(sdl_renderer, font, "Score: " + std::to_string(player->GetScore()), playerInfoPanel.x + 10, playerInfoPanel.y + 100, TEXT_COLOR);
}

void Renderer::RenderScoreTable(std::vector<Player> const &scores)
{
    RenderPanel(sdl_renderer, scoreTablePanel, {220, 220, 220, 255});
    RenderBorder(sdl_renderer, scoreTablePanel, BORDER_COLOR);
    RenderText(sdl_renderer, font, "Score Table", scoreTablePanel.x + 10, scoreTablePanel.y + 10, TEXT_COLOR);
    for (size_t i = 0; i < scores.size(); ++i)
    {
        RenderText(sdl_renderer, font, scores[i].GetName(), scoreTablePanel.x + 10, scoreTablePanel.y + 40 + i * 30, TEXT_COLOR);
        RenderText(sdl_renderer, font, std::to_string(scores[i].GetScore()), scoreTablePanel.x + 150, scoreTablePanel.y + 40 + i * 30, TEXT_COLOR);
    }
}

void Renderer::RenderPlayBoard()
{
    RenderPanel(sdl_renderer, playBoardPanel, {220, 220, 220, 255});
    RenderBorder(sdl_renderer, playBoardPanel, BORDER_COLOR);
}

void Renderer::RenderFood(SDL_Point const &food)
{
    SDL_Rect block;
    block.w = playBoardPanel.w / grid_width;
    block.h = playBoardPanel.h / grid_height;

    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
    block.x = food.x * block.w + playBoardPanel.x;
    block.y = food.y * block.h + playBoardPanel.y;
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::RenderSnakeBody(Snake const snake)
{
    SDL_Rect block;
    block.w = playBoardPanel.w / grid_width;
    block.h = playBoardPanel.h / grid_height;

    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    for (SDL_Point const &point : snake.body)
    {
        block.x = point.x * block.w + playBoardPanel.x;
        block.y = point.y * block.h + playBoardPanel.y;
        SDL_RenderFillRect(sdl_renderer, &block);
    }

    // Render snake's head
    block.x = static_cast<int>(snake.head_x) * block.w + playBoardPanel.x;
    block.y = static_cast<int>(snake.head_y) * block.h + playBoardPanel.y;
    if (snake.alive)
    {
        SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
    }
    else
    {
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    }
    SDL_RenderFillRect(sdl_renderer, &block);
}

void Renderer::UpdateWindowTitle(int score, int fps)
{
    std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
    SDL_SetWindowTitle(sdl_window, title.c_str());
}

void Renderer::RenderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y, SDL_Color color)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect destRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void Renderer::RenderPanel(SDL_Renderer *renderer, SDL_Rect &rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Renderer::RenderBorder(SDL_Renderer *renderer, SDL_Rect &rect, SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}
