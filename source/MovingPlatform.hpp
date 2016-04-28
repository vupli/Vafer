#ifndef MOVINGPLATFORM_H
#define MOVINGPLATFORM_H

#include "Entity.hpp"

class MovingPlatform: public Entity
{
public:
  MovingPlatform(AnimationManager &a,int x,int y):Entity(a,x,y)
  {
    option("MovingPlatform", 0.05f, 0, "move");
  }

  ~MovingPlatform() {}

  void update(float time)
  {
    x += dx * time;
    timer+=time;
    if (timer>6000) {dx*=-1;timer=0;}
    anim.tick(time);
  }

};

#endif
