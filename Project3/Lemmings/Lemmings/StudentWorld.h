#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <unordered_map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class Lemming;

class StudentWorld : public GameWorld
{
public:
    const static int LEMMING_SPAWN_LIMIT = 10;
    const static int LEMMING_SAVE_REQUIREMENT = 5;
    const static int GAME_INITIAL_TICK_COUNT = 2000;

    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();

    virtual int init();
    virtual int move();
    virtual void cleanUp();

    int getTicks() const;

    // World queries
    bool isWallAt(int x, int y) const;
    bool isHazardAt(int x, int y) const;
    Actor* getActorAt(int x, int y) const;
    Actor* getClimbableAt(int x, int y) const;
    Actor* getAttractorAt(int x, int y) const;
    Lemming* getLemmingAt(int x, int y) const;
    bool isEmpty(int x, int y) const;
    void setActorDir(int x, int y, int dir) const;
    bool pheromonePresent(int x, int y, int& direction) const; //sets direction to phermone if conditions are satisified
    bool insideBounds(Coord coord_) const;
    //tools
    bool hasTool(char tool) const;
    bool tryPlaceTool(char tool, Coord c);

    // Lemming accounting
    void incSpawned();
    void incSaved();
    void incDied();
    int getLemmingsSpawned() const;
    int getLemmingsSaved() const;
    int getLemmingsDied() const;

    void spawnLemming(Coord c);


private:
	std::vector<Actor*> m_actors;
    Actor* m_player;

    int m_lemmingsSpawned;
    int m_lemmingsSaved;
    int m_lemmingsDied;

    // Tool inventory
    std::unordered_map<char, int> m_tools;

    int m_tickCount;
};

#endif // STUDENTWORLD_H_
