#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp
// Do not change or remove the createStudentWorld implementation above.

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
}

StudentWorld::~StudentWorld() {
    cleanUp();
}


int StudentWorld::init()
{
    cleanUp();

    Level level(assetPath());
    std::string levelFile = "level";
    if (getLevel() < 10)
        levelFile += "0";
    levelFile += std::to_string(getLevel()) + ".txt";

    Level::LoadResult result = level.loadLevel(levelFile);
    if (result != Level::load_success)
        return GWSTATUS_LEVEL_ERROR;

    for (int x = 0; x < VIEW_WIDTH; x++) {
        for (int y = 0; y < VIEW_HEIGHT; y++) {
            Level::MazeEntry me = level.getContentsOf(Coord(x, y));
            switch (me) {
            case Level::floor:
                m_actors.push_back(new FloorBrick(Coord(x,y), this));
                break;
            case Level::ice_monster:
                m_actors.push_back(new IceMonster(Coord(x, y), this));
                break;
            default:
                break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");

    for (Actor* a : m_actors)
        a->doSomething();
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a : m_actors)
        delete a;
    m_actors.clear();
}

bool StudentWorld::isWallAt(int x, int y) const {
    for (Actor* a : m_actors) {
        if (a->getCoord().x == x && a->getCoord().y == y &&
            a->isFloorBrick())
            return true;
    }
    return false;
}