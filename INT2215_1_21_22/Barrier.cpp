#include "Barrier.h"

INT g_pos[] = { 1250, 1600, 1950, 2300};


Barrier::Barrier()
{
    is_back_ = false;
}

Barrier::~Barrier()
{

}

bool Barrier::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImageFile(path, screen);
    return ret;
}

void Barrier::SetPos(const int& xp, const int& yp)
{
    this->SetRect(xp, yp);
}

void Barrier::SetXPos(const int& xp)
{
    SDL_Rect rect = this->GetRect();
    this->SetRect(xp, rect.y);
}

void Barrier::ShowImg(SDL_Renderer* screen)
{
    this->Render(screen);
}

void Barrier::DoRun(UINT& x_val)
{
    this->rect_.x += x_val;
    if (rect_.x + rect_.w < 0)
    {
        is_back_ = true;
    }
}

DoubleBarrier::DoubleBarrier()
{
    x_val_ = -3;
    is_back_ = false;
    is_pass_ = false;
}

DoubleBarrier::~DoubleBarrier()
{

}

bool DoubleBarrier::Init(SDL_Renderer* screen , const int& xp)
{
    int n = SDLCommonFunction::MyRandom(1, 10);
    int n2 = n;
    n = n * 14;

    bool ret1 = m_Topbarrier.LoadImg("tools//img//top.png", screen);
    bool ret2 = m_Bottombarrier.LoadImg("tools//img//bottom.png", screen);

    if (n2 % 2 == true)
    {
        m_Topbarrier.SetPos(xp, -300 - n);
        m_Bottombarrier.SetPos(xp, GROUND_MAP - 220 - n);
    }
    else
    {
        m_Topbarrier.SetPos(xp, -300 + n);
        m_Bottombarrier.SetPos(xp, GROUND_MAP - 220 + n);
    }

    if (ret1 && ret2)
        return true;
    return false;
}

void DoubleBarrier::RenderImg(SDL_Renderer* screen)
{
    m_Topbarrier.ShowImg(screen);
    m_Bottombarrier.ShowImg(screen);
}

void DoubleBarrier::DoMoving()
{
    m_Topbarrier.DoRun(x_val_);
    m_Bottombarrier.DoRun(x_val_);
    if (m_Topbarrier.GetStateBack() || m_Bottombarrier.GetStateBack())
    {
        is_back_ = true;
    }
}

SDL_Rect DoubleBarrier::GetTopRect()
{
   return m_Topbarrier.GetRectPos();
}

void DoubleBarrier::SetRectVal(const UINT& xp)
{
    m_Topbarrier.SetXPos(xp);
    m_Bottombarrier.SetXPos(xp);
}

void DoubleBarrier::SetIsBack(bool isBack)
{
    is_back_ = isBack;
    m_Topbarrier.SetBack(isBack);
    m_Bottombarrier.SetBack(isBack);
}

void DoubleBarrier::GetRectSlot()
{
    SDL_Rect rect_top = m_Topbarrier.GetRectPos();

    slot_rect_.x = rect_top.x + rect_top.w;
    slot_rect_.y = rect_top.y + rect_top.h;
    slot_rect_.w = 5;
    slot_rect_.h = 160;
}

bool DoubleBarrier::CheckPass(SDL_Rect rect)
{
    bool ret = false;
    ret = SDLCommonFunction::CheckCollision(rect, slot_rect_);
    return ret;
}

bool DoubleBarrier::CheckCol(SDL_Rect pl_rect)
{
    bool ret1 = SDLCommonFunction::CheckCollision(pl_rect, m_Topbarrier.GetRectPos());
    bool ret2 = SDLCommonFunction::CheckCollision(pl_rect, m_Bottombarrier.GetRectPos());

    if (ret1 || ret2)
    {
        return true;
    }

    return false;
}

BarrierManager::BarrierManager()
{
    xp_max_ = 0;
    stop_moving_ = false;
    m_count = 0;
    is_col_ = false;
}

BarrierManager::~BarrierManager()
{
    Free();
}

void BarrierManager::Free()
{
    for (int i = 0; i < m_BarrierList.size(); i++)
    {
        DoubleBarrier* pBarrier = m_BarrierList.at(i);
        delete pBarrier;
        pBarrier = NULL;
    }

    m_BarrierList.clear();
}

bool BarrierManager::InitBarrierList(SDL_Renderer* screen)
{
    DoubleBarrier* m_barrier1 = new DoubleBarrier();
    DoubleBarrier* m_barrier2 = new DoubleBarrier();
    DoubleBarrier* m_barrier3 = new DoubleBarrier();
    DoubleBarrier* m_barrier4 = new DoubleBarrier();

    int ret;
    ret = m_barrier1->Init(screen, g_pos[0]);
    if (ret == false) return false;
    ret = m_barrier2->Init(screen, g_pos[1]);
    if (ret == false) return false;
    ret = m_barrier3->Init(screen, g_pos[2]);
    if (ret == false) return false;
    ret = m_barrier4->Init(screen, g_pos[3]);
    if (ret == false) return false;

    m_BarrierList.push_back(m_barrier1);
    m_BarrierList.push_back(m_barrier2);
    m_BarrierList.push_back(m_barrier3);
    m_BarrierList.push_back(m_barrier4);


    xp_max_ = 3;
    return true;
}

void BarrierManager::SetStopMoving(const bool& stop)
{
    stop_moving_ = stop;
}

void BarrierManager::Render(SDL_Renderer* screen)
{
    for (int i = 0; i < m_BarrierList.size(); i++)
    {
        DoubleBarrier* pBarrier = m_BarrierList.at(i);

        pBarrier->GetRectSlot();

        if (!stop_moving_)
        {
            pBarrier->DoMoving();

            bool ret = pBarrier->GetIsBack();
            if (ret == true)
            {
                DoubleBarrier* endBarrier = m_BarrierList.at(xp_max_);
                SDL_Rect end_rect = endBarrier->GetTopRect();
                UINT xp = end_rect.x + 350;
                pBarrier->SetRectVal(xp);
                pBarrier->SetIsBack(false);
                pBarrier->SetIsPass(false);
                xp_max_ = i;
            }

            bool isCol = pBarrier->CheckCol(player_rect_);
            if (isCol == true)
            {
                is_col_ = true;
                Mix_Chunk* beep_sound = Mix_LoadWAV("tools//sound//punch.wav");
                if (beep_sound != NULL)
                    Mix_PlayChannel(-1, beep_sound, 0);
                break;
            }

            ret = pBarrier->CheckPass(player_rect_);
            if (ret == true)
            {
                if (pBarrier->GetIsPass() == false)
                {
                    Mix_Chunk* beep_sound = Mix_LoadWAV("tools//sound//ting.wav");
                    if (beep_sound != NULL)
                        Mix_PlayChannel(-1, beep_sound, 0);
                    m_count++;
                    pBarrier->SetIsPass(true);
                }
            }
        }

        pBarrier->RenderImg(screen);

    }
}

