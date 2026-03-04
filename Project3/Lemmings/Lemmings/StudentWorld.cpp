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
    m_tickCount = GAME_INITIAL_TICK_COUNT; //initializes ticks per level

    m_lemmingsSpawned = 0;
    m_lemmingsSaved = 0;
    m_lemmingsDied = 0;

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
            case Level::lemming_factory:
                m_actors.push_back(new LemmingFactory(Coord(x, y), this));
                break;
            case Level::bonfire:
                m_actors.push_back(new Bonfire(Coord(x, y), this));
                break;
            case Level::lemming_exit:
                m_actors.push_back(new Exit(Coord(x, y), this));
                break;
            case Level::floor:
                m_actors.push_back(new FloorBrick(Coord(x,y), this));
                break;
            case Level::ice_monster:
                m_actors.push_back(new IceMonster(Coord(x, y), this));
                break;
            case Level::trampoline:
                m_actors.push_back(new Trampoline(Coord(x, y), this));
                break;
            case Level::net:
                m_actors.push_back(new Net(Coord(x, y), this));
                break;
            case Level::pheromone:
                m_actors.push_back(new Pheromone(Coord(x, y), this));
                break;
            case Level::spring:
                m_actors.push_back(new Spring(Coord(x, y), this));
                break;
            case Level::left_one_way_door:
                m_actors.push_back(new OneWayDoor(Coord(x, y), this, GraphObject::left));
                break;
            case Level::right_one_way_door:
                m_actors.push_back(new OneWayDoor(Coord(x, y), this, GraphObject::right));
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

    //setGameStatText("Game will end when you type q");
    m_tickCount--;

    //do something
    m_player->doSomething();
    for (Actor* a : m_actors)
        a->doSomething();

    //remove dead actors
    auto it = m_actors.begin();
    while (it != m_actors.end()) {
        if (!(*it)->isAlive()) {
            delete* it;
            it = m_actors.erase(it);
        }
        else {
            it++;
        }
    }

    //display text
    string tools;
    for (unordered_map<char, int>::iterator it = m_tools.begin(); it != m_tools.end(); it++) {
        if (it->second == 0) continue;
        tools += std::string(it->second, it->first);// it->first * it->second;
    }
    string score (5 - to_string(getScore()).size(), '0');
    score += to_string(getScore());
    string level = (getLevel() < 10) ? "0" + to_string(getLevel()) : to_string(getLevel());
    string lives = to_string(getLives());
    string saved = (m_lemmingsSaved < 10) ? "0" +  to_string(m_lemmingsSaved) : to_string(m_lemmingsSaved);
    string time(4 - to_string(m_tickCount).size(), '0');
    time += to_string(m_tickCount);
    string gameText = "Score: " + score + "  Level: " + level + "  Lives: 0" + lives + "  Saved: ##  Tools: " + tools + "  Time left: " + time;
    setGameStatText(gameText);
    
    //G or more than 5 dead
    int ch;
    getKey(ch);
    if (m_lemmingsDied > LEMMING_SAVE_REQUIREMENT || ch == 'G' || ch == 'g') {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    //If out of time
    if (m_tickCount <= 0) {
       
        if (m_lemmingsSaved < LEMMING_SAVE_REQUIREMENT) {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        else {
            playSound(SOUND_FINISHED_LEVEL);
            increaseScore(m_tickCount);
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    if (m_lemmingsSpawned == LEMMING_SPAWN_LIMIT &&
        m_lemmingsSaved + m_lemmingsDied == LEMMING_SPAWN_LIMIT)
    {
        if (m_lemmingsSaved >= LEMMING_SAVE_REQUIREMENT) {
            playSound(SOUND_FINISHED_LEVEL);
            increaseScore(m_tickCount);
            return GWSTATUS_FINISHED_LEVEL;
        }
        else {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
    }

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

    delete m_player;
    m_player = nullptr;

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
    for (Actor* a : m_actors) {
        if (a->isAlive() &&
            a->isHazard() &&
            a->getCoord().x == x &&
            a->getCoord().y == y)
            return true;
    }
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

Lemming* StudentWorld::getLemmingAt(int x, int y) const {
    for (Actor* a : m_actors) {
        if (a->getCoord().x == x && a->getCoord().y == y && a->isLemming()) {
            return static_cast<Lemming*>(a);
        }
    }
    return nullptr;
}

bool StudentWorld::pheromonePresent(int x, int y, int& direction) const //sets direction to phermone if conditions are satisified
{
    for (int i = 1; i <= 5; i++) {
        if (insideBounds(Coord(x + i, y))) {
            if (getActorAt(x + i, y) != nullptr && getActorAt(x + i, y)->isAttractor()) {
                direction = GraphObject::right;
                return true;
            }
        }
        if (insideBounds(Coord(x - i, y))) {
            if (getActorAt(x - i, y) != nullptr && getActorAt(x - i, y)->isAttractor())
            {
                direction = GraphObject::left;
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::insideBounds(Coord coord_) const {
    if (1 <= coord_.x && coord_.x <= VIEW_WIDTH - 2 &&
        1 <= coord_.y && coord_.y <= VIEW_HEIGHT - 2) {
        return true;
    }
    return false;
}

bool StudentWorld::isEmpty(int x, int y) const {
    return getActorAt(x, y) == nullptr;
}

bool StudentWorld::hasTool(char tool) const {
    if (m_tools.at(tool) > 0) {
        return true;
    }
    return false;
}

bool StudentWorld::tryPlaceTool(char tool, Coord c) {
    if (!hasTool(tool) || !isEmpty(c.x, c.y)) {
        return false;
    }
    switch (tool) {
    case 'T': 
        m_actors.push_back(new Trampoline(c, this));
        break;
    case 'N':
        m_actors.push_back(new Net(c, this));
        break;
    case '<':
        m_actors.push_back(new OneWayDoor(c, this, GraphObject::left));
        break;
    case '>':
        m_actors.push_back(new OneWayDoor(c, this, GraphObject::right));
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

void StudentWorld::incSpawned() { m_lemmingsSpawned++;}
void StudentWorld::incSaved() { m_lemmingsSaved++; }
void StudentWorld::incDied() { m_lemmingsDied++; }
int StudentWorld::getLemmingsSpawned() const {
    return m_lemmingsSpawned;
}
int StudentWorld::getLemmingsSaved() const {
    return m_lemmingsSaved;
}
int StudentWorld::getLemmingsDied() const {
    return m_lemmingsDied;
}

