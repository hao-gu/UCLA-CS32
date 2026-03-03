#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(int imageID, Coord startCoord, StudentWorld* world, int dir)
    : GraphObject(imageID, startCoord, dir), m_world(world)
{
    setVisible(true);
}

Actor::~Actor() {}

//bad implementation, change later
bool Actor::canMove() const{
    return m_world->getTicks() % 10 == 0;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}

Player::Player(Coord startCoord, StudentWorld* world)
    : Actor(IID_PLAYER, startCoord, world) {
}

bool Player::insideBounds(Coord coord_) const {
    if (1 <= coord_.x && coord_.x <= VIEW_WIDTH - 2 &&
        1 <= coord_.y && coord_.y <= VIEW_HEIGHT - 2) {
        return true;
    }
    return false;
}

void Player::doSomething() {
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            //... move builder cursor left ...
            if (insideBounds(getTargetCoord(left)))
                moveTo(left);
            break;
        case KEY_PRESS_RIGHT:
            //... move builder cursor right ...
            if (insideBounds(getTargetCoord(right)))
                moveTo(right);
            break;
        case KEY_PRESS_UP:
            //... move builder cursor right ...
            if (insideBounds(getTargetCoord(up)))
                moveTo(up);
            break;
        case KEY_PRESS_DOWN:
            //... move builder cursor right ...
            if (insideBounds(getTargetCoord(down)))
                moveTo(down);
            break;
        case 'T':
        case 't':
            //... place a trampoline tool ...
            
                //place tool;
            break;
            // etc.
        }
    }
}


FloorBrick::FloorBrick(Coord startCoord, StudentWorld* world)
    : Actor(IID_FLOOR, startCoord, world) {}
void FloorBrick::doSomething() {}
bool FloorBrick::blocksMovement() const { return true; }



IceMonster::IceMonster(Coord startCoord, StudentWorld* world)
    : Actor(IID_ICE_MONSTER, startCoord, world) {
    setDirection(right);
}

void IceMonster::doSomething() {
    if (!canMove()) return; 
    Coord newCoord = getTargetCoord(getDirection());
    int newX = newCoord.x;
    int y = newCoord.y;

    // Check wall in front OR no floor below next square
    if (getWorld()->isWallAt(newX, y) || !getWorld()->isWallAt(newX, y - 1)) {
        setDirection(getDirection() == right ? left : right);
 
        return;
    }

    moveTo(Coord(newX, y));
}
Tool::Tool(int imageID, Coord startCoord, StudentWorld* world, int dir) 
    : Actor(imageID, startCoord, world, dir) {}

Trampoline::Trampoline(Coord startCoord, StudentWorld* world) 
    : Tool(IID_TRAMPOLINE, startCoord, world, right) {}
void Trampoline::doSomething() {}
bool Trampoline::isLaunchable() const { return true; }

Net::Net(Coord startCoord, StudentWorld* world) 
        : Tool(IID_NET, startCoord, world, right) {}
void Net::doSomething() {}
bool Net::isClimbable() const { return true; }


OneWayDoor::OneWayDoor(Coord startCoord, StudentWorld* world, int dir) 
    : Tool(IID_ONE_WAY_DOOR, startCoord, world, dir) {}
void OneWayDoor::doSomething() {}


Pheromone::Pheromone(Coord startCoord, StudentWorld* world)
    :  Tool(IID_PHEROMONE, startCoord, world, right) {}
void Pheromone::doSomething() {}

Spring::Spring(Coord startCoord, StudentWorld* world) 
    : Tool(IID_SPRING, startCoord, world, right) {}
void Spring::doSomething() {}
bool Spring::isLaunchable() const { return true; }