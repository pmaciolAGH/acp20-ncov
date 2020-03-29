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
	std::shared_ptr<std::shared_ptr<bool[]>[]> last_board;
	std::unique_ptr<std::unique_ptr<bool[]>[]> new_board;

	Game(int size) {
		this->size = size;
		last_board = std::make_shared<std::shared_ptr<bool[]>[]>(size);
		new_board = std::make_unique<std::unique_ptr<bool[]>[]>(size);
		for (int i = 0; i < size; i++) {
			last_board[i] = std::make_shared<bool[]>(size);
			new_board[i] = std::make_unique<bool[]>(size);
			for (int j = 0; j < size; j++) {
				last_board[i][j] = false;
				new_board[i][j] = false;
			}
		}
		initialize();
	}

	void make_iteration() {
		print();
		create_board();
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
				if (last_board[m][n] == true) alive++;
			}
		}
		if (last_board[i][j] == true) {
			if (alive < 2 || alive>3) new_board[i][j] = false;
		}
		else {
			if (alive == 3) new_board[i][j] = true;
		}
	}

	void print() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (last_board[i][j]) std::cout << "1 ";
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
			last_board[i][j] = true;
			new_board[i][j] = true;
		}
		file.close();
	}

	void run(int iterations) {
		for (int i = 0; i < iterations; i++) {
			std::cout << "iteration " << i << std::endl;
			make_iteration();
			Sleep(1000);
			system("CLS");
		}
	}

	void create_board() {
		new_board = std::make_unique<std::unique_ptr<bool[]>[]>(size);
		for (int i = 0; i < size; i++) {
			new_board[i] = std::make_unique<bool[]>(size);
			for (int j = 0; j < size; j++) {
				new_board[i][j] = false;
			}
		}
	}
};


int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>(8);
	auto arr = std::make_unique<std::unique_ptr<int[]>[]>(5);
	game->run(20);
	
	return 0;
}
