#include "CommonFunction.h"
#include "Bird.h"
#include "Timer.h"
#include "Barrier.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 640


BaseObject g_bkground;
BaseObject g_ground;

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_MENU = NULL;

bool Init()
{
    bool sucess = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("INT2215_21_22",
                                SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH, SCREEN_HEIGHT,
                                SDL_WINDOW_OPENGL);

    if (g_window != NULL)
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen != NULL)
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR,
                                             RENDER_DRAW_COLOR,
                                             RENDER_DRAW_COLOR,
                                             RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                sucess = false;
        }

        if (TTF_Init() == -1)
        {
            sucess = false;
        }

        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
        {
            sucess = false;
        }

        g_font_text = TTF_OpenFont("tools//font//ARCADE.ttf", 38);
        if (g_font_text == NULL)
        {
            return false;
        }

        g_font_MENU = TTF_OpenFont("tools//font//ARCADE.ttf", 80);
        if (g_font_MENU == NULL)
        {
            return false;
        }
    }

    return sucess;
}


bool LoadBackground()
{
    bool ret = g_bkground.LoadImageFile("tools//img//bkgn.png", g_screen);
    return ret;
}


void close()
{
    g_bkground.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (Init() == false)
    {
        return -1;
    }

    Timer fps;
    bool quit = false;

// khởi tạo Menu game
    int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU, "START", "Exit", "tools//img//MENU.png",-1);
    if (ret_menu == 1) quit = true;

    if (!LoadBackground())
    {
        return -1;
    }

    bool ret = g_ground.LoadImageFile("tools//img//ground2.png", g_screen);
    if (ret == false)
    {
        return -1;
    }
    g_ground.SetRect(0, GROUND_MAP);

again_game: // chơi lại

    Text text_score_, score;
    text_score_.setColor(Text::WHITE_TEXT);
    score.setColor(Text::WHITE_TEXT);

    Bird bird_;
    ret = bird_.LoadImg("tools//img//dot2.png", g_screen);
    bird_.SetRect(300, 250);
    if (ret == false) return -1;

    BarrierManager manage_barrier;
    ret = manage_barrier.InitBarrierList(g_screen);
    if (ret == false) return -1;

    while (!quit)
    {
        fps.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                quit = true;
            }

            bird_.HandleInputAction(g_event, g_screen);
        }

        SDL_RenderClear(g_screen);

        g_bkground.Render(g_screen, NULL);

        manage_barrier.SetPlayerRect(bird_.GetRect());

        bool is_falling = bird_.GetFalling();
        if (is_falling == true)
        {
            manage_barrier.SetStopMoving(true);
        }

        manage_barrier.Render(g_screen);

        bool end_game = manage_barrier.GetColState();
        if (end_game == true)
        {
            bird_.SetFalling(true);
        }
        bird_.DoFalling(g_screen);
        bird_.Show(g_screen);

        int count = manage_barrier.GetCount();
        std::string count_str = std::to_string(count);
        text_score_.SetText(count_str);
        score.SetText("SCORE:");
        text_score_.loadFromRenderedText(g_font_text, g_screen);
        score.loadFromRenderedText(g_font_text, g_screen);
        text_score_.RenderText(g_screen, SCREEN_WIDTH*0.1, 10);
        score.RenderText(g_screen, 10, 10);

        g_ground.Render(g_screen);
        SDL_RenderPresent(g_screen);


        // Khởi tạo menu gameover
        bool game_over = bird_.GetIsDie();
        if (game_over == true)
        {
            Sleep(50);
            int ret_menu = SDLCommonFunction::ShowMenu(g_screen, g_font_MENU,"Play again","Exit","tools//img//MENU END.png", count);
            if (ret_menu == 1)
            {
                quit = true;
                continue;
            }
            else
            {
                manage_barrier.Free();
                goto again_game;
            }
        }

        // Giới hạn tốc độ khung hình
        int val1 = fps.get_ticks();
        if (fps.get_ticks() < 1000 / FRAMES_PER_SECOND)
        {
            SDL_Delay((1000 / FRAMES_PER_SECOND) - fps.get_ticks());
        }
    }

    close();
    return 0;
}

