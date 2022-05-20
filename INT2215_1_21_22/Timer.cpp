#include "Timer.h"
#include "CommonFunction.h"

Timer::Timer()
{
  start_tick_ = 0;
  paused_ticks_ = 0;
  is_paused_ = false;
  is_started_ = false;
}

void Timer::start()
{
  is_started_ = true;
  is_paused_ = false;
  start_tick_ = SDL_GetTicks();
}

void Timer::stop()
{
  is_started_ = false;
  is_paused_ = false;
}

void Timer::pause()
{
  if( ( is_started_ == true ) && ( is_paused_ == false ) )
  {
    is_paused_ = true;
    paused_ticks_ = SDL_GetTicks() - start_tick_;
  }
}

void Timer::unpause()
{
  if( is_paused_ == true )
  {
    is_paused_ = false;
    start_tick_ = SDL_GetTicks() - paused_ticks_;
    paused_ticks_ = 0;
  }
}

int Timer::get_ticks()
{
  if( is_started_ == true )
  {
    if( is_paused_ == true )
    {
      return paused_ticks_;
    }
    else
    {
      return SDL_GetTicks() - start_tick_;
    }
  }

  return 0;
}

bool Timer::is_started()
{
  return is_started_;
}

bool Timer::is_paused()
{
  return is_paused_;
}

