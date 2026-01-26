#ifndef GAME_H
#define GAME_H

class Arena;

#include <string>
using namespace std;

bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
int computeDanger(const Arena& a, int r, int c);

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();

    // Mutators
    void play();

private:
    Arena* m_arena;
    int    m_turns;

    // Helper functions
    string takePlayerTurn();
};

#endif