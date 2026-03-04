#include <iostream>
#include <string>
#include <stack>

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

bool pathExists(string maze[], int nRows, int nCols, int r, int c, int er, int ec) {
    cerr << "(" << r << ", " << c << ")" << endl;
    if (r == er && c == ec) return true;
    maze[r][c] = 'V';
    int move[4][2] = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };
    for (int i = 0; i < 4; i++) {
        if (valid(r + move[i][0], c + move[i][1], nRows, nCols, maze)) {
            if (pathExists(maze, nRows, nCols, r + move[i][0], c + move[i][1], er, ec))
                return true;
        }
    }
    return false;
}


/*int main()
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