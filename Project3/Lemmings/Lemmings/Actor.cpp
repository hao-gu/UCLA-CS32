#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp


Actor::Actor(int imageID, Coord startCoord, StudentWorld* world, int dir, bool alive)
    : GraphObject(imageID, startCoord, dir), m_world(world), m_alive(alive)
{
    setVisible(true);
}

Actor::~Actor() {}

//bad implementation, change later
bool Actor::canMove(int ticks) const{
    return m_world->getTicks() % ticks == 0;
}

StudentWorld* Actor::getWorld() const {
    return m_world;
}

bool Actor::isAlive() const {
    return m_alive;
}
void Actor::setDead() {
    m_alive = false;
}

Player::Player(Coord startCoord, StudentWorld* world)
    : Actor(IID_PLAYER, startCoord, world) {
}


void Player::doSomething() {
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
        case KEY_PRESS_LEFT:
            //... move builder cursor left ...
            if (getWorld()->insideBounds(getTargetCoord(left)))
                moveTo(left);
            break;
        case KEY_PRESS_RIGHT:
            //... move builder cursor right ...
            if (getWorld()->insideBounds(getTargetCoord(right)))
                moveTo(right);
            break;
        case KEY_PRESS_UP:
            //... move builder cursor right ...
            if (getWorld()->insideBounds(getTargetCoord(up)))
                moveTo(up);
            break;
        case KEY_PRESS_DOWN:
            //... move builder cursor right ...
            if (getWorld()->insideBounds(getTargetCoord(down)))
                moveTo(down);
            break;
        case 'T':
        case 't':
            getWorld()->tryPlaceTool('T', getCoord());
            break;
        case 'N':
        case 'n':
            getWorld()->tryPlaceTool('N', getCoord());
            break;
     
        case 'P':
        case 'p':
            getWorld()->tryPlaceTool('P', getCoord());
            break;
        case 'S':
        case 's':
            getWorld()->tryPlaceTool('S', getCoord());
            break;
        case '<':
            getWorld()->tryPlaceTool('<', getCoord());
            break;
        case '>':
            getWorld()->tryPlaceTool('>', getCoord());
            break;
        default:
            break;
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
    Lemming* l = getWorld()->getLemmingAt(getCoord().x, getCoord().y);
    if (l != nullptr) {
        l->killLemming();
        return;
    }
    
    if (!canMove(ICE_MONSTER_TICKS)) return; 
    
    Coord newCoord = getTargetCoord(getDirection());
    int newX = newCoord.x;
    int y = newCoord.y;

    // Check wall in front OR no floor below next square
    if (getWorld()->isWallAt(newX, y) || !getWorld()->isWallAt(newX, y - 1)) {
        setDirection(getDirection() == right ? left : right);
 
        return;
    }

    moveTo(newCoord);
}

Tool::Tool(int imageID, Coord startCoord, StudentWorld* world, int dir) 
    : Actor(imageID, startCoord, world, dir) {
    setVisible(true);
}

Trampoline::Trampoline(Coord startCoord, StudentWorld* world) 
    : Tool(IID_TRAMPOLINE, startCoord, world, right) {}
void Trampoline::doSomething() {
    //does this bounce multiple lemmings or just one?
    Lemming* l = getWorld()->getLemmingAt(getCoord().x, getCoord().y);
    if (l != nullptr && l->getState() != bouncing) {
        l->setState(bouncing);
        l->setBounceDistance(0);
        int dist = l->getFallDistance();
        l->setBounceTarget(dist > 0 ? dist - 1 : 0);
        getWorld()->playSound(SOUND_BOUNCE);
    }
}
bool Trampoline::isLaunchable() const { return true; }

Net::Net(Coord startCoord, StudentWorld* world) 
        : Tool(IID_NET, startCoord, world, right) {}
void Net::doSomething() {}
bool Net::isClimbable() const { return true; }


OneWayDoor::OneWayDoor(Coord startCoord, StudentWorld* world, int dir) 
    : Tool(IID_ONE_WAY_DOOR, startCoord, world, dir) {}
void OneWayDoor::doSomething() {
    Coord c = getCoord();
    getWorld()->setActorDir(c.x, c.y, getDirection()); //sets a's direction to doors direction
}


Pheromone::Pheromone(Coord startCoord, StudentWorld* world)
    :  Tool(IID_PHEROMONE, startCoord, world, right) {}
void Pheromone::doSomething() {}

Spring::Spring(Coord startCoord, StudentWorld* world) 
    : Tool(IID_SPRING, startCoord, world, right) {}
void Spring::doSomething() {
    Lemming* l = getWorld()->getLemmingAt(getCoord().x, getCoord().y);
    if (l != nullptr && l->getState() != bouncing) {
        l->setState(bouncing);
        l->setBounceDistance(0);
        l->setBounceTarget(SPRING_BOUNCE_DIST);
        getWorld()->playSound(SOUND_BOUNCE);
    }
}
bool Spring::isLaunchable() const { return true; }


LemmingFactory::LemmingFactory(Coord startCoord, StudentWorld* world)
    : Actor(IID_LEMMING_FACTORY, startCoord, world, right) {
}
void LemmingFactory::doSomething() {
    if (!canMove(LEMMING_FACTORY_TICKS) || getWorld()->getTicks() > StudentWorld::GAME_INITIAL_TICK_COUNT - LEMMING_FACTORY_TICKS) return;
    if (getWorld()->getLemmingsSpawned() < StudentWorld::LEMMING_SPAWN_LIMIT) {
        getWorld()->spawnLemming(getCoord());
        getWorld()->incSpawned();
    }
}


Lemming::Lemming(Coord startCoord, StudentWorld* world, LemmingState state)
    : Actor(IID_LEMMING, startCoord, world, right), m_state(state),
    m_fallDistance(0), m_bounceTarget(0), m_bounceDistance(0) {}

void Lemming::setState(LemmingState state) {
    m_state = state;
}
bool Lemming::switchToClimb(Coord c) {
    Actor* a = getWorld()->getClimbableAt(c.x, c.y);
    if (a!=nullptr) {
        m_state = climbing;
        return true;
    }
    return false;
}

bool Lemming::checkNextWall(Coord c) {
    if (getWorld()->isWallAt(c.x, c.y)) {
        setDirection(getDirection() == right ? left : right);
        return true;
    }
    return false;
}

bool Lemming::moveUp(Coord c) {
    if (getWorld()->isWallAt(c.x, c.y)) {
        return false;
    }
    moveTo(c);
    return true;
}

void Lemming::doSomething() {
    if (!isAlive()) return;
    if (!canMove(everyNTicks(m_state))) return;

    Coord here = getCoord();

    int dir;
    if (getWorld()->pheromonePresent(here.x, here.y, dir)) {
        setDirection(dir);
    }


    switch (m_state) {
    case walking:
        // check fall
    {
        //1.check current square climbable
        if (switchToClimb(here)) return;
        //2&3. check next wall
        Coord next = getTargetCoord(here, getDirection());
        if (checkNextWall(next)) return;
        //4. check belowNext. If no wall, change to falling. Either way move to next coord
        if (!getWorld()->isWallAt(next.x, next.y - 1)) {
            m_state = falling;
            m_fallDistance = 0;
        }
        moveTo(next);
        return;
    }
    case falling:
    {
        //1.check current square climbable
        if (switchToClimb(here)) return;
        //2. check below
        Coord below = getTargetCoord(here, down);
        if (getWorld()->isWallAt(below.x, below.y)) {
            if (m_fallDistance > MAXIMUM_FALL_DISTANCE) {
                killLemming();
                return;
            }
            m_state = walking;
            return;
        }

        m_fallDistance++;
        moveTo(below);
        return;
    }
    case climbing:
    {
        //1.check current square climbable
        if (!switchToClimb(here)) {
            m_state = walking;
            return;
        }
        Coord above = getTargetCoord(here, up);
        moveUp(above);
        return;
    }
    case bouncing:
        // 1.check current square climbable
    {
        if (switchToClimb(here)) return;

        Coord above = getTargetCoord(here, up);
            
        bool apex = false;
        if (moveUp(above)) {
            return;
        }
        else apex = true;

        if (apex || m_bounceTarget <= m_bounceDistance) {
            Coord next = getTargetCoord(here, getDirection());
            if (!checkNextWall(next))
                moveTo(next);
            m_fallDistance = 0;
            m_state = falling;
            return;
        }
        return;
    }
    default:
        break;
    }
}

int Lemming::everyNTicks(LemmingState state) {
    if (state == walking) return LEMMING_WALK_TICKS;
    return LEMMING_OTHER_TICKS;
} //4 for walking, 2 for other actions
void Lemming::saveLemming() {
    getWorld()->playSound(SOUND_LEMMING_SAVED);
    getWorld()->incSaved();
    getWorld()->increaseScore(SCORE_SAVED_LEMMING);
    setDead();
}
void Lemming::killLemming() {
    getWorld()->playSound(SOUND_LEMMING_DIE);
    getWorld()->incDied();
    setDead();
}

Lemming::LemmingState Lemming::getState() const {
    return m_state;
}

int Lemming::getFallDistance() const {
    return m_fallDistance;
}

int Lemming::getBounceTarget() const {
    return m_bounceTarget;
}

int Lemming::getBounceDistance() const {
    return m_bounceDistance;
}

void Lemming::setFallDistance(int d) {
    m_fallDistance = d;
}

void Lemming::setBounceTarget(int y) {
    m_bounceTarget = y;
}

void Lemming::setBounceDistance(int d) {
    m_bounceDistance = d;
}

void Lemming::incBounceDistance() {
    m_bounceDistance++;
}

Bonfire::Bonfire(Coord startCoord, StudentWorld* world)
    : Actor(IID_BONFIRE, startCoord, world, right) {
}
void Bonfire::doSomething() {
    //increaseAnimationNumber();

    while (getWorld()->getLemmingAt(getCoord().x, getCoord().y) != nullptr) // will check for alive 
    {
        Lemming* a = getWorld()->getLemmingAt(getCoord().x, getCoord().y);
        if (a->isLemming()) {
            a->killLemming();
        }
    }
} //kill lemmings

Exit::Exit(Coord startCoord, StudentWorld* world)
    : Actor(IID_EXIT, startCoord, world, right) {
}
void Exit::doSomething() {
    Lemming* l = getWorld()->getLemmingAt(getCoord().x, getCoord().y);
    if (l != nullptr) {
        l->saveLemming();
        return;
    }
} //saves 1 lemming per tick



