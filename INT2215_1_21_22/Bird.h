#ifndef BIRD_H_
#define BIRD_H_
#include "CommonFunction.h"

class Bird : public BaseObject
{
public:
  Bird();
  ~Bird();

  bool LoadImg(std::string path, SDL_Renderer* screen);
  void Show(SDL_Renderer* des);
  void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
  void Jump();
  void DoFalling(SDL_Renderer* des);
  bool GetFalling() const { return is_falling_; }
  void SetFalling(bool falling) { is_falling_ = falling; }
  void DoGround(SDL_Renderer* screen);
  bool GetIsDie() const { return is_die_; }
private:
  bool is_falling_;
  float x_val_;
  float y_val_;
  bool is_die_;
};


#endif

