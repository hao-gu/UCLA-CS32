#include <iostream>
#include "Arena.h"
#include "Zombie.h"
#include "Player.h"
#include "globals.h"
using namespace std;

Arena::Arena(int nRows, int nCols)
    : m_rows(nRows), m_cols(nCols), m_player(nullptr), m_nZombies(0), m_history(nRows, nCols)
{
    if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
            << nCols << "!" << endl;
        exit(1);
    }
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    for (int k = 0; k < m_nZombies; k++)
        delete m_zombies[k];
    delete m_player;
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::zombieCount() const
{
    return m_nZombies;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c, "Arena::getCellStatus");
    return m_grid[r - 1][c - 1];
}

int Arena::numberOfZombiesAt(int r, int c) const
{
    int count = 0;
    for (int k = 0; k < m_nZombies; k++)
    {
        Zombie* zp = m_zombies[k];
        if (zp->row() == r && zp->col() == c)
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;

    // Fill displayGrid with dots (empty) and stars (poisoned brains)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

    // Indicate each zombie's position
    for (int k = 0; k < m_nZombies; k++)
    {
        const Zombie* zp = m_zombies[k];
        char& gridChar = displayGrid[zp->row() - 1][zp->col() - 1];
        switch (gridChar)
        {
        case '.':  gridChar = 'Z'; break;
        case 'Z':  gridChar = '2'; break;
        case '9':  break;
        default:   gridChar++; break;  // '2' through '8'
        }
    }

    // Indicate the player's position
    if (m_player != nullptr)
        displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? '#' : '@');

    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r - 1][c - 1];
        cout << endl;
    }
    cout << endl;

    // Write message, zombie, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << zombieCount() << " zombies remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c, "Arena::setCellStatus");
    m_grid[r - 1][c - 1] = status;
}

bool Arena::addZombie(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a zombie on a spot with a poisoned brain
    if (getCellStatus(r, c) != EMPTY)
        return false;

    // Don't add a zombie on a spot with a player
    if (m_player != nullptr && m_player->row() == r && m_player->col() == c)
        return false;

    if (m_nZombies == MAXZOMBIES)
        return false;
    m_zombies[m_nZombies] = new Zombie(this, r, c);
    m_nZombies++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (!isPosInBounds(r, c))
        return false;

    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;

    // Don't add a player on a spot with a zombie
    if (numberOfZombiesAt(r, c) > 0)
        return false;

    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveZombies()
{
    // Move all zombies
    for (int k = m_nZombies - 1; k >= 0; k--)
    {
        Zombie* zp = m_zombies[k];
        zp->move();

        if (m_player != nullptr &&
            zp->row() == m_player->row() && zp->col() == m_player->col())
            m_player->setDead();

        if (zp->isDead())
        {
            delete zp;

            // The order of Zombie pointers in the m_zombies array is
            // irrelevant, so it's easiest to move the last pointer to
            // replace the one pointing to the now-deleted zombie.  Since
            // we are traversing the array from last to first, we know this
            // last pointer does not point to a dead zombie.

            m_zombies[k] = m_zombies[m_nZombies - 1];
            m_nZombies--;
        }
    }
}

History& Arena::history() {
	return m_history;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1 && r <= m_rows && c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c, string functionName) const
{
    if (!isPosInBounds(r, c))
    {
        cout << "***** " << "Invalid arena position (" << r << ","
            << c << ") in call to " << functionName << endl;
        exit(1);
    }
}