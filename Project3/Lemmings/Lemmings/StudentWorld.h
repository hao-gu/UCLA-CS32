#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual ~StudentWorld();

  virtual int init();
  virtual int move();
  virtual void cleanUp();

  bool isWallAt(int x, int y) const;

private:
	std::vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
