#include "CommonFunction.h"

int SDLCommonFunction::ShowMenu(SDL_Renderer* g_screen, TTF_Font* font,
                                const std::string& menu1,
                                const std::string& menu2,
                                const std::string& img_name,
                                const int count)
{
    char* c1 = (char*)menu1.c_str();
    char* c2 = (char*)menu2.c_str();
    char* img_file = (char*)img_name.c_str();

    int sz1 = menu1.length();
    int sz2 = menu2.length();

    int time = 0;
    int x = 0;
    int y = 0;
    const int k_num = 2;
    char* labels[k_num];

    labels[0] = new char [sz1 + 1];
    labels[1] = new char[sz2 + 1];

    strcpy_s(labels[0], sz1+1, c1);
    strcpy_s(labels[1], sz2+1, c2);

    SDL_Texture* menu[k_num];
    bool selected[k_num] = { 0, 0 };
    SDL_Color color[2] = { { 255, 255, 10 },{ 255, 0, 0 } };

    Text text_object[k_num];
    text_object[0].SetText(labels[0]);
    text_object[0].setColor(color[0].r, color[0].g, color[0].b);
    text_object[0].loadFromRenderedText(font, g_screen);
    text_object[1].SetText(labels[1]);
    text_object[1].setColor(color[0].r, color[0].g, color[0].b);
    text_object[1].loadFromRenderedText(font, g_screen);


    SDL_Rect pos[k_num];
    pos[0].x = SCREEN_WIDTH*0.4 - 70;
    pos[0].y = SCREEN_HEIGHT*0.8 -110;
    pos[1].x = SCREEN_WIDTH*0.4 - 70;
    pos[1].y = SCREEN_HEIGHT*0.8 + 150;

    BaseObject gBackground;
    bool ret = gBackground.LoadImageFile(img_file, g_screen);

    SDL_Event event;
    while (1)
    {
        time = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                text_object[0].Free();
                text_object[0].Free();
                gBackground.Free();
                return 1;
            case SDL_MOUSEMOTION:
                x = event.motion.x;
                y = event.motion.y;
                for (int i = 0; i < k_num; ++i)
                {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        if (!selected[i])
                        {
                            selected[i] = 1;
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[1].r, color[1].g, color[1].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                    else
                    {
                        if (selected[i])
                        {
                            selected[i] = 0;
                            text_object[i].Free();
                            text_object[i].SetText(labels[i]);
                            text_object[i].setColor(color[0].r, color[0].g, color[0].b);
                            text_object[i].loadFromRenderedText(font, g_screen);
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                x = event.button.x;
                y = event.button.y;
                for (int i = 0; i < k_num; ++i) {
                    if (x >= pos[i].x && x <= pos[i].x + pos[i].w &&
                        y >= pos[i].y && y <= pos[i].y + pos[i].h)
                    {
                        text_object[0].Free();
                        text_object[1].Free();
                        gBackground.Free();
                        return i;
                    }
                }
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    text_object[0].Free();
                    text_object[1].Free();
                    gBackground.Free();
                    return 1;
                }
            }
        }

        gBackground.Render(g_screen, NULL);

        if (count != -1) {
            Text text_score_, score;
            text_score_.setColor(color[0].r, color[0].g, color[0].b);
            score.setColor(color[0].r, color[0].g, color[0].b);
            std::string count_str = std::to_string(count);
            score.SetText("YOUR SCORE : ");
            text_score_.SetText(count_str);
            score.loadFromRenderedText(font, g_screen);
            text_score_.loadFromRenderedText(font, g_screen);
            score.RenderText(g_screen, SCREEN_WIDTH*0.3 , SCREEN_HEIGHT*0.8 -190);
            text_score_.RenderText(g_screen, SCREEN_WIDTH*0.7, SCREEN_HEIGHT*0.8 -190);
        }

        for (int i = 0; i < k_num; ++i)
        {
            text_object[i].RenderText(g_screen, pos[i].x, pos[i].y);
            pos[i].w = text_object[i].getWidth();
            pos[i].h = text_object[i].getHeight();
        }


        SDL_RenderPresent(g_screen);
        if (1000 / 30 > (SDL_GetTicks() - time))
            SDL_Delay(1000 / 30 - (SDL_GetTicks() - time));
    }

    return 0;
}


int SDLCommonFunction::MyRandom(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

bool SDLCommonFunction::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
  int left_a = object1.x;
  int right_a = object1.x + object1.w;
  int top_a = object1.y;
  int bottom_a = object1.y + object1.h;

  int left_b = object2.x;
  int right_b = object2.x + object2.w;
  int top_b = object2.y;
  int bottom_b = object2.y + object2.h;

  if (left_a > left_b && left_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (left_a > left_b && left_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (top_a > top_b && top_a < bottom_b)
    {
      return true;
    }
  }

  if (right_a > left_b && right_a < right_b)
  {
    if (bottom_a > top_b && bottom_a < bottom_b)
    {
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (left_b > left_a && left_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (top_b > top_a && top_b < bottom_a)
    {
      return true;
    }
  }

  if (right_b > left_a && right_b < right_a)
  {
    if (bottom_b > top_a && bottom_b < bottom_a)
    {
      return true;
    }
  }

  if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
  {
    return true;
  }

  return false;
}



