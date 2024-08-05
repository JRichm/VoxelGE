#include <iostream>
#include <string>
#include <vector>


#include <cstdlib>

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


int WIDTH = 30;
int HEIGHT = 30;

std::vector<std::vector<std::string>> grid(WIDTH, std::vector<std::string>(HEIGHT, ". "));

void displayGrid() {
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			std::cout << grid[w][h];
		}
		std::cout << std::endl;
	}
}

int main() {
	grid[15][15] = "* ";

	while (true) {
		displayGrid();

		char command;
		std::cin >> command;

		switch (command) {
			case 'q':
				return 0;
			default:
				break;
		}

		return 1;
	}
}