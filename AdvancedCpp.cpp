// AdvancedCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <memory>


struct Board {
	int size;
	bool** cells;

	Board(int size) {
		this->size = size;
		cells = new bool*[size];
		for (int i = 0; i < size; i++) {
			cells[i] = new bool[size];
			for (int j = 0; j < size; j++) {
				cells[i][j] = false;
			}
		}
	}
};

struct Game {
	int size;
	std::unique_ptr<Board> last_board;
	std::unique_ptr<Board> new_board;

	Game(int size) {
		this->size = size;
		last_board = std::make_unique<Board>(size);
		new_board = std::make_unique<Board>(size);
		initialize();
	}

	void make_iteration() {
		print();
		new_board = std::make_unique<Board>(size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				check_element(i, j);
			}
		}
		last_board = std::move(new_board);
	}

	void check_element(int i, int j) {
		int alive = 0;
		for (int m = i - 1; m <= i + 1; m++) {
			for (int n = j - 1; n <= j + 1; n++) {
				if (m < 0 || m >= size || n < 0 || n >= size || (m == i && n == j)) continue;
				if (last_board->cells[m][n] == true) alive++;
			}
		}
		if (last_board->cells[i][j] == true) {
			if (alive < 2 || alive>3) new_board->cells[i][j] = false;
		}
		else {
			if (alive == 3) new_board->cells[i][j] = true;
		}
	}

	void print() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (last_board->cells[i][j]) std::cout << "1 ";
				else std::cout << "0 ";
			}
			std::cout << std::endl;
		}
	}

	void initialize() {
		std::fstream file;
		int i, j;
		file.open("data.txt");
		while (!file.eof()) {
			file >> i >> j;
			last_board->cells[i][j] = true;
			new_board->cells[i][j] = true;
		}
		file.close();
	}

	void run(int iterations) {
		for (int i = 0; i < iterations; i++) {
			int x;
			std::cout << "iteration " << i << std::endl;
			make_iteration();
			Sleep(1000);
			//std::cin >> x;
			system("CLS");
		}
	}
};



int number_of_iterations = 100;

int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>(8);
	game->run(20);
	
	/*cell** board = new cell*[board_size];
	for (int i = 0; i < board_size; i++) {
		board[i] = new cell[board_size];
	}
	initialize_board(board);
	for (int i = 0; i < number_of_iterations; i++) {
		std::cout << "iteration " << i << std::endl;
		make_iteration(board);
		Sleep(1000);
		system("CLS");
	}*/
	return 0;
}

/*void make_iteration(cell** board) {
	print(board);
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			check_element(i, j, board);
		}
	}
	process(board);
}

void check_element(int i, int j, cell** board) {
	int alive = 0;
	for (int m = i - 1; m <= i + 1; m++) {
		for (int n = j - 1; n <= j + 1; n++) {
			if (m < 0 || m >= board_size || n < 0 || n >= board_size || (m == i && n == j)) continue;
			if (board[m][n].last_state == true) alive++;
		}
	}
	if (board[i][j].last_state == true) {
		if (alive < 2 || alive>3) board[i][j].current_state = false;
	}
	else {
		if (alive == 3) board[i][j].current_state = true;
	}
}

void process(cell** board) {
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			board[i][j].last_state = board[i][j].current_state;
		}
	}
}

void initialize_board(std::unique_ptr<Board> board) {
	std::fstream file;
	int i, j;
	file.open("data.txt");
	while (!file.eof()) {
		file >> i >> j;
		board[i][j].current_state = true;
		board[i][j].last_state = true;
	}
	file.close();
}

void print(cell** board) {
	for (int i = 0; i < board_size; i++) {
		for (int j = 0; j < board_size; j++) {
			std::cout << board[i][j].current_state << " ";
		}
		std::cout << std::endl;
	}
}*/