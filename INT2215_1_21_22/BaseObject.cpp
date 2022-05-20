#include "BaseObject.h"

BaseObject::BaseObject()
{
    p_object_ = NULL;
    rect_.x = 0;
    rect_.y = 0;
    rect_.w = 0;
    rect_.h = 0;
}

BaseObject::~BaseObject()
{
    Free();
}

bool BaseObject::LoadImageFile(std::string path, SDL_Renderer* screen)
{
    Free();
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface != NULL)
    {
        // color key
        Uint32 key = SDL_MapRGB(loadedSurface->format,
                                COLOR_KEY_R,
                                COLOR_KEY_G,
                                COLOR_KEY_B);
        SDL_SetColorKey(loadedSurface, SDL_TRUE, key);

        // Tạo kết cấu từ các pixel bề mặt
        newTexture = SDL_CreateTextureFromSurface(screen, loadedSurface);
        if (newTexture != NULL)
        {
            // Đưa vào kích thước ảnh
            rect_.w = loadedSurface->w;
            rect_.h = loadedSurface->h;
        }

        SDL_FreeSurface(loadedSurface);
    }

    p_object_ = newTexture;
    return p_object_ != NULL;
}

// Phân bổ bộ nhớ
void BaseObject::Free()
{
    if (p_object_ != NULL)
    {
        SDL_DestroyTexture(p_object_);
        p_object_ = NULL;
        rect_.w = 0;
        rect_.h = 0;
    }
}

void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip )
{
    SDL_Rect renderQuad = { rect_.x, rect_.y, rect_.w, rect_.h };
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }
    SDL_RenderCopy(des, p_object_, clip, &renderQuad);
}

