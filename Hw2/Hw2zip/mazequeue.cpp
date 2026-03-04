#include <iostream>
#include <string>
#include <queue>
using namespace std;

class Coord
{
public:
    Coord(int r, int c) : m_r(r), m_c(c) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool valid(int r, int c, int nRows, int nCols, string maze[]) {
    if (r < 0 || r >= nRows || c < 0 || c >= nCols) return false;
    if (maze[r][c] != '.') return false;
    maze[r][c] = 'V';
    return true;
}

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue <Coord> coordstack;
    coordstack.push(Coord(sr, sc));
    maze[sr][sc] = 'V';
    while (!coordstack.empty()) {
        Coord curr = coordstack.front();
        cerr << "(" << curr.r() << ", " << curr.c() << ")" << endl;
        coordstack.pop();
        if (curr.r() == er && curr.c() == ec) { return true; }
        if (valid(curr.r(), curr.c() - 1, nRows, nCols, maze)) {
            coordstack.push(Coord(curr.r(), curr.c() - 1));
        }
        if (valid(curr.r() + 1, curr.c(), nRows, nCols, maze)) {
            coordstack.push(Coord(curr.r() + 1, curr.c()));
        }
        if (valid(curr.r(), curr.c() + 1, nRows, nCols, maze)) {
            coordstack.push(Coord(curr.r(), curr.c() + 1));
        }
        if (valid(curr.r() - 1, curr.c(), nRows, nCols, maze)) {
            coordstack.push(Coord(curr.r() - 1, curr.c()));
        }
    }
    if (maze[er][ec] == 'V') return true;
    return false;
}

/*
int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X.X..X...X",
        "X....XXX.X",
        "X.XXXX.X.X",
        "X......XXX",
        "X.XX.X...X",
        "X.X..X.X.X",
        "X.X.XXXX.X",
        "X.X...X..X",
        "XXXXXXXXXX"
    };

    if (pathExists(maze, 10, 10, 4, 6, 1, 1))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}*/