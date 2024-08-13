#include <iostream>
#include <string>
#include <vector>

#include <windows.h>

#include <cstdlib>
#include <conio.h>



// i need to output a matrix to the console
// 1. first need to display just zeros, in a visual way
// 2. need to figure out how to change specific values
// 3. figure out how to change a specific value of the matrix
// 4. set up different functions for each cell of the matrix to change others
//		maybe set up a timer to actually see the changes
// 5. set up the simulation


// setting up the program to display the matrixes at first may cause some headache
// when trying to display the matrix in open opengl
// think about how we want to keep track of every cell, what other games do to handle
// the large amount of pixels to keep track of
// thinking of this, we should have a way of recording all of the cells of the matrix
// and a separate part of the code that displays the cells.
// this way we can work on each part of the engine and advance the techniques individually 


// how can we display text to the console?
// test and figure out how


// CONWAYS GAME OF LIFE RULES

	// 1) any live cell with < 2 live neighbors dies (underpopulation)
	// 2) any live cell with two or three live neighbors lives
	// 3) any live cell with more than three live neighbors dies (overpopulation)
	// 4) any dead cell with exactly three live neighbors becomes a live cell (reproduction)



class Grid {
private:
	std::vector<std::string> grid;
	std::vector<std::string> bufferGrid;
	int width, height;

public:
	Grid(int w, int h) : width(w), height(h) {
		grid = clearedGrid(w, h);
		bufferGrid = clearedGrid(w, h);

		bufferGrid[25][24 * 2] = '*';
		bufferGrid[25][24 * 2 + 1] = ' ';

		bufferGrid[25][25 * 2] = '*';
		bufferGrid[25][25 * 2 + 1] = ' ';

		bufferGrid[25][26 * 2] = '*';
		bufferGrid[25][26 * 2 + 1] = ' ';

		bufferGrid[24][25 * 2] = '*';
		bufferGrid[24][25 * 2 + 1] = ' ';

		bufferGrid[26][26 * 2] = '*';
		bufferGrid[26][26 * 2 + 1] = ' ';

		grid = bufferGrid;
	};

	std::vector<std::string> clearedGrid(int w, int h) {
		std::vector<std::string> clearedGrid = std::vector<std::string>(h, std::string(w * 2, '.'));
		for (auto& row : clearedGrid) {
			for (int i = 0; i < w; ++i) {
				row[i * 2] = '.';
				row[i * 2 + 1] = ' ';
			}
		};
		return clearedGrid;
	};

	std::string getCell(int x, int y) const {
		if (x >= 0 && x < height && y >= 0 && y < width) {
			return grid[x].substr(y * 2, 2);
		}
		return "";
	};

	void setCell(int x, int y, char value) {
		if (x >= 0 && x < height && y >= 0 && y < width) {
			bufferGrid[x][y * 2] = value;
			bufferGrid[x][y * 2 + 1] = ' ';
		}
	}

	int countNeighbors(int x, int y) {
		int count = 0;
		for (int i = -1; i <= 1; ++i) {
			for (int j = -1; j <= 1; ++j) {
				if (i == 0 && j == 0) continue;
				if (getCell(x + i, y + j) == "* ") {
					count++;
				}
			}
		}
		return count;
	}

	void displayGrid() const {
		system("cls");
		for (const auto& row : grid) {
			std::cout << row << '\n';
		}
	}

	void updateGrid() {
		bufferGrid = clearedGrid(height, width);

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				int count = countNeighbors(i, j);
				std::string currentCell = getCell(i, j);

				if (currentCell == "* ") {
					if (count < 2 || count > 3) {
						setCell(i, j, '.');
					} else {
						setCell(i, j, '*');
					}
				} else {
					if (count == 3) {
						setCell(i, j, '*');
					}
				}
			}
		}
		grid = bufferGrid;
	}
};

int main2() {

	Grid grid(50, 50);

	grid.displayGrid();

	while (true) {
		grid.updateGrid();
		grid.displayGrid();
		Sleep(1);
	}
	return 1;
}
