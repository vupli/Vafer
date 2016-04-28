#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Animation.hpp"
#include "level.hpp"

using namespace std;
using namespace sf;

class Entity
{
public:
  float x, y, dx, dy, w, h;
  AnimationManager anim;
  vector<Object> obj;
  bool life, dir;
  float timer, timer_end;
  string Name;
  int Health;

  Entity(AnimationManager &A, int X, int Y)
  {
    anim = A;
    x = X;
    y = Y;
    dir = 0;

    life=true;
    timer=0;
    timer_end=0;
    dx=dy=0;
  }

  virtual ~Entity() {

  }

  virtual void update(float __attribute__((unused)) time) = 0;

  void draw(RenderWindow &window)
  {
    anim.draw(window,static_cast<int>(x), static_cast<int>(y+h));
  }

  FloatRect getRect()
  {
    return FloatRect(x,y,w,h);
  }

  void option(string NAME, float SPEED=0, int HEALTH=10, string FIRST_ANIM="")
  {
    Name = NAME;
    if (FIRST_ANIM!="") anim.set(FIRST_ANIM);
    w = anim.getW();
    h = anim.getH();
    dx = SPEED;
    Health = HEALTH;
  }

};


#endif
