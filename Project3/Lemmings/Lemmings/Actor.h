#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

#include "GraphObject.h"

class StudentWorld;

class Actor : public GraphObject {
public:
    Actor(int imageID, Coord startCoord, StudentWorld* world, int dir = right);

    virtual ~Actor();

    virtual void doSomething() = 0;

    virtual bool isFloorBrick() const;

    StudentWorld* getWorld() const;

private:
    StudentWorld* m_world;
};

class FloorBrick : public Actor {
public:
    FloorBrick(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    bool isFloorBrick() const;
};

class IceMonster : public Actor {
public:
    IceMonster(Coord startCoord, StudentWorld* world);
    virtual void doSomething();

private:
    int m_direction; // left or right
};

#endif // ACTOR_H_
