#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <unordered_map>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
public:
    const int SPAWN_LIMIT = 10;
    const int SAVE_REQUIREMENT = 5;

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
    bool isEmpty(int x, int y) const;

    bool isPheromone(Coord c, int& direction) const; //sets direction to phermone if conditions are satisified

    //tools
    bool hasTool(char tool) const;
    bool tryPlaceTool(char tool, Coord c);

    // Lemming accounting
    void incSpawned();
    void incSaved();
    void incDied();


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
