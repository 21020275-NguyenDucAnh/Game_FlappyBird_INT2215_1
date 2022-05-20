#ifndef TIMER_H_
#define TIMER_H_

class Timer
{
public:
  Timer();
  void start();
  void stop();
  void pause();
  void unpause();
  int get_ticks();
  bool is_started();
  bool is_paused();

private:
  int start_tick_;
  int paused_ticks_;
  bool is_paused_;
  bool is_started_;
};

#endif

