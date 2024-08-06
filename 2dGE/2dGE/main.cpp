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


const int GRID_SIZE = 30;

std::vector<std::vector<std::string>> grid(GRID_SIZE, std::vector<std::string>(GRID_SIZE, ". "));


void displayGrid(std::string grid[GRID_SIZE][GRID_SIZE]) {
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			std::cout << grid[i][j] << ' ';
		}
		std::cout << std::endl;
	}
}

int x = 15;
int y = 15;

int main() {
	std::string grid[GRID_SIZE][GRID_SIZE] = {};
	for (int i = 0; i < GRID_SIZE; ++i) {
		for (int j = 0; j < GRID_SIZE; ++j) {
			grid[i][j] = ". ";
		}
	}
	grid[x][y] = '* ';

	while (true) {

		system("cls");

		if (_kbhit()) {
			char command = _getch();
			std::cout << command << std::endl;

			switch (command) {
				case 'q':
					return 0;
				case 'w':
					y = (y > 0) ? y - 1 : y;
					break;
				case 'a':
					x = (x > 0) ? x - 1 : x;
					break;
				case 's':
					y = (y < GRID_SIZE - 1) ? y + 1 : y;
					break;
				case 'd':
					x = (x < GRID_SIZE - 1) ? x + 1 : x;
					break;
			}

			grid[x][y] = "* ";
		}

		displayGrid(grid);
		std::cout << x << ',' << y << std::endl;

		Sleep(500);

	}
	return 1;
}