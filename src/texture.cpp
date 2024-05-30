#include <iostream>
#include "texture.h"

LTexture::LTexture(SDL_Renderer *sdl_renderer, TTF_Font *font)
    : sdl_renderer(sdl_renderer), font(font),
      mTexture(NULL), mWidth(0), mHeight(0) {}

LTexture::~LTexture()
{
    // Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture *newTexture = NULL;

    // Load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image " << path << std::endl;
        std::cerr << "SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(sdl_renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cerr << "Unable to create texture from " << path << std::endl;
            std::cerr << "SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    // Get rid of preexisting texture
    free();

    // Render text surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface != NULL)
    {
        // Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(sdl_renderer, textSurface);
        if (mTexture == NULL)
        {
            std::cerr << "Unable to create texture from rendered text!" << std::endl;
            std::cerr << "SDL_ttf: " << SDL_GetError() << std::endl;
        }
        else
        {
            // Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
            text = textureText;
        }

        // Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    else
    {
        std::cerr << "Unable to render text surface!" << std::endl;
        std::cerr << "SDL_ttf: " << TTF_GetError() << std::endl;
    }

    // Return success
    return mTexture != NULL;
}
#endif

void LTexture::free()
{
    // Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(sdl_renderer, mTexture, clip, &renderQuad, angle, center, flip);
    rect = renderQuad;
}

void LTexture::render(int x, int y, int w, int h)
{
    SDL_Rect renderQuad = {x, y, w, h};
    rect = renderQuad;
    SDL_RenderCopyEx(sdl_renderer, mTexture, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}

SDL_Rect LTexture::getRect() {
    return rect;
}

// Get text
std::string LTexture::getText()
{
    return text;
}