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
    font = TTF_OpenFont("../font/lazy.ttf", 28);
    if (nullptr == font)
    {
        std::cerr << "Failed to load font!\n";
        std::cerr << "TTF_Error: " << TTF_GetError() << "\n";
        throw std::runtime_error("Failed to load font!");
    }

    // Load food texture
    foodTexture = std::make_unique<LTexture>(sdl_renderer, nullptr);
    if (!foodTexture->loadFromFile("../images/food.png"))
    {
        std::cerr << "Error: Could not load food texture.\n";
        throw std::runtime_error("Could not load food texture.");
    }
}

Renderer::~Renderer()
{
    SDL_DestroyWindow(sdl_window);
    // Quit SDL subsystems
    TTF_Quit();
    // IMG_Quit();
    SDL_Quit();
}

void Renderer::Render(Snake const &snake, SDL_Point const &food, Player *player, std::vector<Player> const &scores)
{
    // Clear screen
    SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
    SDL_RenderClear(sdl_renderer);
    // Render Player Info panel
    RenderPlayerInfo(player);
    // Render Score Table panel
    RenderScoreTable(scores);
    // Render play board
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
    RenderText(sdl_renderer, font, "Health: " + std::to_string(player->GetHealth()), playerInfoPanel.x + 10, playerInfoPanel.y + 130, TEXT_COLOR);
}

void Renderer::RenderScoreTable(std::vector<Player> const &scores)
{
    RenderPanel(sdl_renderer, scoreTablePanel, {220, 220, 220, 255});
    RenderBorder(sdl_renderer, scoreTablePanel, BORDER_COLOR);
    RenderText(sdl_renderer, font, "Score Table", scoreTablePanel.x + 10, scoreTablePanel.y + 10, TEXT_COLOR);
    for (size_t i = 0; i < scores.size() && i < 10; ++i)
    {
        RenderText(sdl_renderer, font, std::to_string(i + 1), scoreTablePanel.x + 10, scoreTablePanel.y + 40 + i * 30, TEXT_COLOR);
        RenderText(sdl_renderer, font, scores[i].GetName(), scoreTablePanel.x + 40, scoreTablePanel.y + 40 + i * 30, TEXT_COLOR);
        RenderText(sdl_renderer, font, std::to_string(scores[i].GetScore()), scoreTablePanel.x + 210, scoreTablePanel.y + 40 + i * 30, TEXT_COLOR);
    }
}

void Renderer::RenderPlayBoard()
{
    RenderPanel(sdl_renderer, playBoardPanel, {220, 220, 220, 255});
    RenderBorder(sdl_renderer, playBoardPanel, BORDER_COLOR);
}

void Renderer::RenderFood(SDL_Point const &food)
{
    // Calculate the food position on the play board
    int x = food.x * playBoardPanel.w / grid_width + playBoardPanel.x;
    int y = food.y * playBoardPanel.h / grid_height + playBoardPanel.y;

    // Render the food image
    foodTexture->render(x, y, playBoardPanel.w / grid_width, playBoardPanel.h / grid_height);
}

void Renderer::RenderSnakeBody(Snake const &snake)
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
    LTexture texture(sdl_renderer, font);
    if (!texture.loadFromRenderedText(text.c_str(), color))
    {
        std::cerr << "Failed to render " << text << " text!\n";
        std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    }
    texture.render(x, y);
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

std::pair<std::string, std::string> Renderer::RenderEnterPlayerWindow()
{
    bool quit = false;
    std::string inputText;
    SDL_Event e;
    SDL_Color textColor = {0x0, 0x0, 0x0, 0xFF};

    // Initialize text textures
    LTexture nameLable(sdl_renderer, font);
    LTexture nameTextBox(sdl_renderer, font);

    LTexture autoPlayLable(sdl_renderer, font);
    LTexture autoPlayTextBox(sdl_renderer, font);

    LTexture logo(sdl_renderer, font);
    LTexture instruction(sdl_renderer, font);

    // Load textures
    if (!nameLable.loadFromRenderedText("Enter your name:", textColor) ||
        !nameTextBox.loadFromRenderedText("Player 01", textColor) ||
        !autoPlayLable.loadFromRenderedText("Auto Play (Yes/No):", textColor) ||
        !autoPlayTextBox.loadFromRenderedText("Yes", textColor) ||
        !logo.loadFromFile("../images/logo.jpg") ||
        !instruction.loadFromRenderedText("Instruction: Close this window to enter to the Game!", textColor))
    {
        // Handle error
        return std::make_pair("", "");
    }

    // Initialize textboxes
    std::vector<LTexture *> textboxes = {&nameTextBox, &autoPlayTextBox};
    LTexture *currentTextbox = nullptr;

    // Enable text input
    SDL_StartTextInput();

    // While application is running
    while (!quit)
    {
        bool renderText = false;
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                // Handle backspace
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0)
                {
                    inputText.pop_back();
                    renderText = true;
                }
                // Handle copy
                else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL)
                {
                    SDL_SetClipboardText(inputText.c_str());
                }
                // Handle paste
                else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL)
                {
                    char *tempText = SDL_GetClipboardText();
                    inputText = tempText;
                    SDL_free(tempText);

                    renderText = true;
                }
            }
            else if (e.type == SDL_TEXTINPUT)
            {
                // Not copy or pasting
                if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
                {
                    inputText += e.text.text;
                    renderText = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {

                // Check if the user has clicked on a textbox
                for (LTexture *textbox : textboxes)
                {
                    SDL_Rect rect = textbox->getRect();

                    if (e.button.x >= rect.x && e.button.x <= rect.x + rect.w &&
                        e.button.y >= rect.y && e.button.y <= rect.y + rect.h)
                    {
                        // Set the current textbox to the one that was clicked
                        currentTextbox = textbox;
                        break;
                    }
                }
            }
        }

        if (renderText && currentTextbox != nullptr)
        {
            if (inputText != "")
            {
                currentTextbox->loadFromRenderedText(inputText.c_str(), textColor);
            }
            else
            {
                currentTextbox->loadFromRenderedText(" ", textColor);
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(sdl_renderer);

        // Render text textures
        renderTextTexture(logo, (screen_width - logo.getWidth()) / 2, 50);
        renderTextTexture(nameLable, logo);
        renderTextTexture(nameTextBox, nameLable);
        renderTextTexture(autoPlayLable, nameTextBox);
        renderTextTexture(autoPlayTextBox, autoPlayLable);
        renderTextTexture(instruction, autoPlayTextBox);

        // Update screen
        SDL_RenderPresent(sdl_renderer);
    }

    // Disable text input
    SDL_StopTextInput();

    return std::make_pair(nameTextBox.getText(), autoPlayTextBox.getText());
}

void Renderer::renderTextTexture(LTexture &texture, int x, int y)
{
    texture.render(x, y);
}

void Renderer::renderTextTexture(LTexture &texture, LTexture &top)
{
    int x = (screen_width - texture.getWidth()) / 2;
    int y = top.getRect().y + top.getHeight() + 10;
    texture.render(x, y);
}