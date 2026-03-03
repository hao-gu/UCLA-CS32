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
    m_tools['T'] = 0;
    m_tools['N'] = 0;
    m_tools['<'] = 0;
    m_tools['>'] = 0;
    m_tools['P'] = 0;
    m_tools['S'] = 0;
    for (char tool : level.getTools()) {
        m_tools[tool]++;
    }

    m_player = new Player(Coord(VIEW_WIDTH / 2, VIEW_HEIGHT / 2), this);



    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you type q

    setGameStatText("Game will end when you type q");
    m_tickCount++;

    //do something
    m_player->doSomething();
    for (Actor* a : m_actors)
        a->doSomething();
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::getTicks() const {
    return m_tickCount;
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
            a->blocksMovement())
            return true;
    }
    return false;
}

bool StudentWorld::isHazardAt(int x, int y) const { 
    return false; 
}

Actor* StudentWorld::getActorAt(int x, int y) const {
    for (Actor* a : m_actors) {
        if (a->getCoord().x == x && a->getCoord().y == y) {
            return a;
        }
    }
    return nullptr;
}

bool StudentWorld::hasTool(char tool) const {
    if (m_tools.at(tool) > 0) {
        return true;
    }
    return false;
}

bool StudentWorld::tryPlaceTool(char tool, Coord c) {
    if (!hasTool('T') || getActorAt(c.x, c.y) != nullptr) {
        return false;
    }
    int imID;
    switch (tool) {
    case 'T': 
        m_actors.push_back(new Trampoline(c, this));
        break;
    case 'N':
        m_actors.push_back(new Net(c, this));
        break;
    case '<':
        m_actors.push_back(new OneWayDoor(c, this, left));
        break;
    case '>':
        m_actors.push_back(new OneWayDoor(c, this, right));
        break;
    case 'P':
        m_actors.push_back(new Pheromone(c, this));
        break;
    case 'S':
        m_actors.push_back(new Spring(c, this));
        break;
    }
    m_tools.at(tool)--;
    return true;
}