#include "History.h"
#include "Arena.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 0; r < nRows; r++)
		for (int c = 0; c < nCols; c++)
			m_grid[r][c] = EMPTY;
}

bool History::record(int r, int c)
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols) {
		return false;
	}
	m_grid[r - 1][c - 1]++;
	return true;
}

void History::display() const
{
	clearScreen();

	for (int r = 0; r < m_rows; r++)
	{
		for (int c = 0; c < m_cols; c++)
		{
			int status = m_grid[r][c];
			if (status == EMPTY)
				cout << '.';
			else if (status >= 1 && status <= 8)
				cout << (char) ('A' + status - 1);
			else
				cout << 'Z';
		}
		cout << endl;
	}
	cout << endl;
}