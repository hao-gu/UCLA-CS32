#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(int imageID, Coord startCoord, StudentWorld* world, int dir)
    : GraphObject(imageID, startCoord, dir), m_world(world)
{
    setVisible(true);
}

Actor::~Actor() {}

bool Actor::isFloorBrick() const { return false; }

StudentWorld* Actor::getWorld() const {
    return m_world;
}

// ---------------- FloorBrick ----------------

FloorBrick::FloorBrick(Coord startCoord, StudentWorld* world)
    : Actor(IID_FLOOR, startCoord, world) {}
void FloorBrick::doSomething() {}
bool FloorBrick::isFloorBrick() const { return true; }

// ---------------- IceMonster ----------------

IceMonster::IceMonster(Coord startCoord, StudentWorld* world)
    : Actor(IID_ICE_MONSTER, startCoord, world), m_direction(right) {}

void IceMonster::doSomething() {
    int dx = (m_direction == right ? 1 : -1);
    int newX = getCoord().x + dx;
    int y = getCoord().y;

    // Check wall in front OR no floor below next square
    if (getWorld()->isWallAt(newX, y) || !getWorld()->isWallAt(newX, y - 1)) {
        m_direction = (m_direction == right ? left : right);
        setDirection(m_direction);
        return;
    }

    moveTo(Coord(newX, y));
}