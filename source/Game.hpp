﻿#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <list>
#include "level.hpp"
#include "Animation.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "MovingPlatform.hpp"
#include "HealthBar.hpp"

#include <fstream>

using namespace sf;

void RunGame()
{
  int screenW, screenH;
  bool screenSize;
  std::ifstream resolutoinFile("resolutoin.txt");
  resolutoinFile.operator >>(screenSize);
  if(screenSize){
      screenW = 800;
      screenH = 600;
    }
  else {
      screenW = 1280;
      screenH = 720;
    }

  RenderWindow window(VideoMode(screenW, screenH), "Vafer");

  View view( FloatRect(0, 0, screenW, screenH) );

  Level lvl;
  lvl.LoadFromFile("files/Level99.tmx");

  Music music;
  music.setLoop(true);
  music.openFromFile("files/music.ogg");
  bool sounds;
  std::ifstream soundFile("sound.txt");
  soundFile >> sounds;
  if(sounds){
      music.play();
    }

  Texture enemy_t, moveplatform_t, megaman_t, bullet_t, bg;
  bg.loadFromFile("files/images/bg4.png");
  enemy_t.loadFromFile("files/images/enemy2.png");
  moveplatform_t.loadFromFile("files/images/movingPlatform.png");
  megaman_t.loadFromFile("files/images/megaman.png");
  bullet_t.loadFromFile("files/images/bullet.png");


  AnimationManager anim;
  anim.loadFromXML("files/anim_megaman.xml",megaman_t);
  anim.animList["jump"].loop = 0;

  AnimationManager anim2;
  anim2.create("move",bullet_t,7,10,8,8,1,0);
  anim2.create("explode",bullet_t,27,7,18,18,4,0.01,29,false);

  AnimationManager anim3;
  //  anim3.create("move",enemy_t,0,0,16,16,2,0.002,18);
  //  anim3.create("dead",enemy_t,58,0,16,16,1,0);
  anim3.create("move",enemy_t,1,0,50,50,2,0.002,51);
  anim3.create("dead",enemy_t,104,0,50,50,1,0.002,51,0);

  AnimationManager anim4;
  anim4.create("move",moveplatform_t,0,0,95,22,1,0);

  Sprite background(bg);
  background.setOrigin(bg.getSize().x/2,bg.getSize().y/2);

  std::list<Entity*>  entities;
  std::list<Entity*>::iterator it;

  std::vector<Object> e = lvl.GetObjects("enemy");
  for (int i=0;i < e.size();i++)
    entities.push_back(new ENEMY(anim3, lvl, e[i].rect.left, e[i].rect.top) );

  e = lvl.GetObjects("MovingPlatform");
  for (int i=0;i < e.size();i++)
    entities.push_back(new MovingPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top) );

  Object pl = lvl.GetObject("player");
  PLAYER Player(anim, lvl, pl.rect.left, pl.rect.top);

  HealthBar healthBar;

  Clock clock;

  /////////////////// ???????? ????  /////////////////////
  while (window.isOpen())
    {
      float time = clock.getElapsedTime().asMicroseconds();
      clock.restart();

      time = time/400;

      if (time > 40) time = 40;

      Event event;
      while (window.pollEvent(event))
        {
          if (event.type == Event::Closed)
            window.close();

          if (event.type == Event::KeyPressed)
            if (event.key.code==Keyboard::Space)
              entities.push_back(new Bullet(anim2,lvl,Player.x+18,Player.y+15,Player.dir) );
        }


      if (Keyboard::isKeyPressed(Keyboard::Left)) Player.key["L"]=true;
      if (Keyboard::isKeyPressed(Keyboard::Right)) Player.key["R"]=true;
      if (Keyboard::isKeyPressed(Keyboard::Up)) Player.key["Up"]=true;
      if (Keyboard::isKeyPressed(Keyboard::Down)) Player.key["Down"]=true;
      if (Keyboard::isKeyPressed(Keyboard::Space)) Player.key["Space"]=true;


      for(it=entities.begin();it!=entities.end();)
        {
          Entity *b = *it;
          b->update(time);
          if (b->life==false)	{ it  = entities.erase(it); delete b;}
          else it++;
        }


      Player.update(time);
      healthBar.update(Player.Health);


      for(it=entities.begin();it!=entities.end();it++)
        {
          //1. ?????
          if ((*it)->Name=="Enemy")
            {
              Entity *enemy = *it;

              if (enemy->Health<=0) continue;

              if  (Player.getRect().intersects( enemy->getRect() ))
                if (Player.dy>0) { enemy->dx=0; Player.dy=-0.2; enemy->Health=0;}
                else if (!Player.hit) { Player.Health-=5; Player.hit=true;
                    if (Player.dir) Player.x+=10; else Player.x-=10;}


              for (std::list<Entity*>::iterator it2=entities.begin(); it2!=entities.end(); it2++)
                {
                  Entity *bullet = *it2;
                  if (bullet->Name=="Bullet")
                    if ( bullet->Health>0)
                      if  (bullet->getRect().intersects( enemy->getRect() ) )
                        { bullet->Health=0; enemy->Health-=5;}
                }
            }

          //2. ?????????? ?????????
          if ((*it)->Name=="MovingPlatform")
            {
              Entity *movPlat = *it;
              if  (Player.getRect().intersects( movPlat->getRect() ) )
                if (Player.dy>0)
                  if (Player.y+Player.h<movPlat->y+movPlat->h)
                    {Player.y=movPlat->y - Player.h + 3; Player.x+=movPlat->dx*time; Player.dy=0; Player.STATE=PLAYER::stay;}
            }

          //3.. ? ?.?.
        }


      /////////////////////?????????? ?? ?????/////////////////////
      view.setCenter( Player.x,Player.y);
      window.setView(view);

      background.setPosition(view.getCenter());
      window.draw(background);

      lvl.Draw(window);

      for(it=entities.begin();it!=entities.end();it++)
        (*it)->draw(window);

      Player.draw(window);
      healthBar.draw(window);

      window.display();
    }


}

#endif GAME_H
