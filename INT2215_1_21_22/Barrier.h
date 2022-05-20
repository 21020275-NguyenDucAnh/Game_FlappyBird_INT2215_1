#ifndef BARRIER_H_
#define BARRIER_H_

#include "CommonFunction.h"

class Barrier : BaseObject
{
public:
    Barrier();
    ~Barrier();
    bool LoadImg(std::string path, SDL_Renderer* screen);
    void SetPos(const int& xp, const int& yp);
    void SetXPos(const int&xp);
    SDL_Rect GetRectPos() const { return this->GetRect(); }
    void ShowImg(SDL_Renderer* screen);
    void DoRun(UINT& x_val);
    bool GetStateBack() const { return is_back_; }
    void SetBack(bool b) { is_back_ = b; }
private:
    bool is_back_;
    int x_pos_;
    int y_pos_;
};



class DoubleBarrier
{
public:
    DoubleBarrier();
    ~DoubleBarrier();
    bool Init(SDL_Renderer* screen, const int& xp);
    void RenderImg(SDL_Renderer* screen);
    void DoMoving();
    bool GetIsBack() const { return is_back_; }
    void SetIsBack(bool isBack);
    SDL_Rect GetTopRect();
    void SetRectVal(const UINT& xp);
    void GetRectSlot();
    bool CheckPass(SDL_Rect rect);
    bool GetIsPass() const { return is_pass_; }
    void SetIsPass(const bool& pp) { is_pass_ = pp; }
    bool CheckCol(SDL_Rect pl_rect);
private:
    Barrier m_Topbarrier;
    Barrier m_Bottombarrier;
    UINT x_val_;
    bool is_back_;
    SDL_Rect slot_rect_;
    bool is_pass_;
};


class BarrierManager
{
public:
    BarrierManager();
    ~BarrierManager();
    bool InitBarrierList(SDL_Renderer* screen);
    void Render(SDL_Renderer* screen);
    void SetStopMoving(const bool& stop);
    void SetPlayerRect(SDL_Rect pRect) { player_rect_ = pRect; }
    bool GetColState() const { return is_col_; }
    int GetCount() const { return m_count; }
    void Free();
private:
    std::vector<DoubleBarrier*> m_BarrierList;
    UINT xp_max_;
    bool stop_moving_;
    SDL_Rect player_rect_;
    int m_count;
    bool is_col_;
};
#endif

