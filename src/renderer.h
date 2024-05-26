#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include "snake.h"
#include "player.h"
#include "texture.h"

const SDL_Color TEXT_COLOR = {0, 0, 0, 255};     // Black text color
const SDL_Color BG_COLOR = {255, 255, 255, 255}; // White background color
const SDL_Color BORDER_COLOR = {0, 0, 0, 255};   // Black border color

class Renderer
{
public:
    Renderer(const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();

    void Render(Snake const snake, SDL_Point const &food, Player *layer, std::vector<Player> const &players);
    std::string RenderEnterPlayerWindow();
    void UpdateWindowTitle(int score, int fps);

private:
    SDL_Window *sdl_window;
    SDL_Renderer *sdl_renderer;
    TTF_Font *font;
    SDL_Rect playerInfoPanel = {10, 10, 250, 150};
    SDL_Rect scoreTablePanel = {10, 170, 250, 200};
    SDL_Rect playBoardPanel = {270, 10, 640, 640};

    const std::size_t screen_width = playerInfoPanel.w + playBoardPanel.w + 30;
    const std::size_t screen_height = playBoardPanel.h + 20;
    const std::size_t grid_width;
    const std::size_t grid_height;

    void RenderPlayerInfo(Player *player);
    void RenderScoreTable(std::vector<Player> const &players);
    void RenderPlayBoard();
    void RenderFood(SDL_Point const &food);
    void RenderSnakeBody(Snake const snake);

    void RenderText(SDL_Renderer *renderer, TTF_Font *font, const std::string &text, int x, int y, SDL_Color color);
    void RenderPanel(SDL_Renderer *renderer, SDL_Rect &rect, SDL_Color color);
    void RenderBorder(SDL_Renderer *renderer, SDL_Rect &rect, SDL_Color color);
};

#endif