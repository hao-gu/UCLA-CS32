#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

#include "GraphObject.h"

class StudentWorld;
 

class Actor : public GraphObject {
public:
    enum LemmingState {
        walking, bouncing, falling, climbing
    };

    Actor(int imageID, Coord startCoord, StudentWorld* world, int dir = right, bool alive = true);

    virtual ~Actor();

    virtual void doSomething() = 0;

    StudentWorld* getWorld() const;

    bool canMove() const;
    bool isAlive() const;
    //do i need a dead member var

    virtual bool blocksMovement() const { return false; }
    virtual bool isClimbable() const { return false; }
    virtual bool isLaunchable() const { return false; }
    virtual bool isLemming() const { return false; }
    virtual bool isHazard() const { return false; }
    virtual bool isExit() const { return false; }
    virtual bool isTool() const { return false; }

private:
    StudentWorld* m_world;
    bool m_alive;
};

class Player: public Actor {
public:
    Player(Coord startCoord, StudentWorld* world);
    bool insideBounds(Coord coord_) const;
    virtual void doSomething();
};

class FloorBrick : public Actor {
public:
    FloorBrick(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    bool blocksMovement() const;
};

class IceMonster : public Actor {
public:
    IceMonster(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
};

class LemmingFactory : public Actor {
public:
    LemmingFactory(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
};

class Lemming : public Actor {
public:
    Lemming(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    int everyNTicks(LemmingState m_state); //4 for walking, 2 for other actions
    virtual bool isLemming() const { return true; }
    void saveLemming();
    void killLemming();
private:
    LemmingState m_state;
    int m_fallDistance;
    int m_bounceTarget;
    int m_bounceDistance;
};

class Bonfire : public Actor {
public:
    Bonfire(Coord startCoord, StudentWorld* world);
    virtual void doSomething(); //kill lemmings
};

class Exit : public Actor {
public:
    Exit(Coord startCoord, StudentWorld* world);
    virtual void doSomething(); //saves 1 lemming per tick
};

//tools

class Tool : public Actor {
public:
    Tool(int imageID, Coord startCoord, StudentWorld* world, int dir);
    //abstract base class
};

class Trampoline : public Tool {
public:
    Trampoline(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    virtual bool isLaunchable() const;
};

class Net : public Tool {
public:
    Net(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    virtual bool isClimbable() const;
};

class OneWayDoor : public Tool {
public:
    OneWayDoor(Coord startCoord, StudentWorld* world, int dir);
    virtual void doSomething();
};

class Pheromone : public Tool {
public:
    Pheromone(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
};

class Spring : public Tool {
public:
    Spring(Coord startCoord, StudentWorld* world);
    virtual void doSomething();
    virtual bool isLaunchable() const;
};

#endif // ACTOR_H_
