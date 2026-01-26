#include <iostream>
#include "Game.h"
#include "Arena.h"
#include "Zombie.h"
#include "Player.h"
#include "globals.h"
#include <string>
using namespace std;


//functions

bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
    default:  return false;
    case 'n': dir = NORTH; break;
    case 'e': dir = EAST;  break;
    case 's': dir = SOUTH; break;
    case 'w': dir = WEST;  break;
    }
    return true;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poisoned brain and
// not move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // How dangerous is it to stand?
    int standDanger = computeDanger(a, r, c);

    // if it's not safe, see if moving is safer
    if (standDanger > 0)
    {
        int bestMoveDanger = standDanger;
        int bestMoveDir = NORTH;  // arbitrary initialization

        // check the four directions to see if any move is
        // better than standing, and if so, record the best
        for (int dir = 0; dir < NUMDIRS; dir++)
        {
            int rnew = r;
            int cnew = c;
            if (attemptMove(a, dir, rnew, cnew))
            {
                int danger = computeDanger(a, rnew, cnew);
                if (danger < bestMoveDanger)
                {
                    bestMoveDanger = danger;
                    bestMoveDir = dir;
                }
            }
        }

        // if moving is better than standing, recommend move
        if (bestMoveDanger < standDanger)
        {
            bestDir = bestMoveDir;
            return true;
        }
    }
    return false;  // recommend standing
}

int computeDanger(const Arena& a, int r, int c)
{
    // Our measure of danger will be the number of zombies that might move
    // to position (r,c).  If a zombie is in that position, it is fatal,
    // so a large value is returned.

    if (a.numberOfZombiesAt(r, c) > 0)
        return MAXZOMBIES + 1;

    int danger = 0;
    if (r > 1)
        danger += a.numberOfZombiesAt(r - 1, c);
    if (r < a.rows())
        danger += a.numberOfZombiesAt(r + 1, c);
    if (c > 1)
        danger += a.numberOfZombiesAt(r, c - 1);
    if (c < a.cols())
        danger += a.numberOfZombiesAt(r, c + 1);

    return danger;
}

Game::Game(int rows, int cols, int nZombies)
{
    if (nZombies < 0)
    {
        cout << "***** Cannot create Game with negative number of zombies!" << endl;
        exit(1);
    }
    if (nZombies > MAXZOMBIES)
    {
        cout << "***** Trying to create Game with " << nZombies
            << " zombies; only " << MAXZOMBIES << " are allowed!" << endl;
        exit(1);
    }
    int nEmpty = rows * cols - nZombies - 1;  // 1 for Player
    if (nEmpty < 0)
    {
        cout << "***** Game created with a " << rows << " by "
            << cols << " arena, which is too small to hold a player and "
            << nZombies << " zombies!" << endl;
        exit(1);
    }

    // Create arena
    m_arena = new Arena(rows, cols);

    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);

    // Populate with zombies
    while (nZombies > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addZombie(r, c);
        nZombies--;
    }

    m_turns = 0;
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x/h or nothing): ";
        string playerMove;
        getline(cin, playerMove);

        Player* player = m_arena->player();
        int dir;

        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonedBrain();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonedBrain();
            else if (tolower(playerMove[0]) == 'h') {
                m_arena->history().display();
                cout << "Press enter to continue";
				cin.ignore(10000, '\n');
                return "h";
            }
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x/h." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    Player* player = m_arena->player();
    if (player == nullptr)
        return;
    while (!player->isDead() && m_arena->zombieCount() > 0)
    {
        string msg = takePlayerTurn();
        if (msg != "h") {
            m_turns++;
            if (!player->isDead())
                m_arena->moveZombies();
            m_arena->display(msg);
            cout << m_turns << " turns have been taken." << endl;
        }
        else {
			m_arena->display("");
        }
    }
    if (player->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}



