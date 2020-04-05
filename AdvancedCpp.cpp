// AdvancedCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <memory>


class Board {
public:
	int size;
	bool** cells;

	Board(int size) {
		std::cout << "Constructor" << std::endl;
		this->size = size;
		cells = new bool*[size];
		for (int i = 0; i < size; i++) {
			cells[i] = new bool[size];
			for (int j = 0; j < size; j++) {
				cells[i][j] = false;
			}
		}
	}

	Board& operator=(Board&& other) {
		std::cout << "Move operator" << std::endl;
		for (int i = 0; i < size; i++) {
			delete cells[i];
		}
		delete[] cells;
		size = other.size;
		cells = other.cells;

		other.size = 0;
		other.cells = nullptr;
		return *this;
	}

	Board(Board&& other) :
		size(other.size),
		cells(std::move(other.cells))
	{
		std::cout << "Move constructor" << std::endl;
		other.cells = nullptr;
		other.size = 0;
	}
	
};

class Game {
public:
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
			if (alive == 2 || alive==3) new_board->cells[i][j] = true;
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
			std::cout << "iteration " << i << std::endl;
			make_iteration();
			Sleep(200);
			system("CLS");
		}
	}

};
using namespace std;

Board create_board(int val) {
	return Board(val);
}

void move_semantics_test() {
	int x;
	Board board(6);
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	board = create_board(5);
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	Board board2(std::move(board));
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	std::cout << "board2" << std::endl;
	std::cout << "Object address: " << &board2 << "  " << "Array address: " << board2.cells << "  ";
	std::cout << "Size value: " << board2.size << std::endl;
	std::cin >> x;
}

int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>(8);
	game->run(10);
	//move_semantics_test();
	return 0;
}

