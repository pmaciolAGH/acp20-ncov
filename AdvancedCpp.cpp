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
	std::unique_ptr<bool**> cells;

	Board() {
		size = 0;
		cells = nullptr;
	}

	Board(int size) {
		std::cout << "Constructor" << std::endl;
		this->size = size;
		bool** tmp = new bool*[size];
		for (int i = 0; i < size; i++) {
			tmp[i] = new bool[size];
			for (int j = 0; j < size; j++) {
				tmp[i][j] = false;
			}
		}
		cells = std::make_unique<bool**>(tmp);
	}

	Board& operator=(Board&& other) {
		std::cout << "Move operator" << std::endl;
		cells = std::move(other.cells);
		size = other.size;

		other.size = 0;
		return *this;
	}

	Board(Board&& other) :
		size(other.size),
		cells(std::move(other.cells))
	{
		std::cout << "Move constructor" << std::endl;
		other.size = 0;
	}

};

Board create_board(int val) {
	return Board(val);
}

class MooreNeighborhood {
public:
	int size;
	std::shared_ptr<bool*> cells;
	bool if_cell_alive;

	MooreNeighborhood(Board& board, int x, int y) {
		if_cell_alive = board.cells.get()[0][x][y];
		bool* tmp = new bool[8];
		size = 0;
		for (int i = x - 1; i <= x + 1; i++) {
			for (int j = y - 1; j <= y + 1; j++) {
				if (i < 0 || i >= board.size || j < 0 || j >= board.size) continue;
				if (i == x && j == y) {
					if_cell_alive = board.cells.get()[0][i][j];
				}
				else {
					tmp[size] = board.cells.get()[0][i][j];
					size++;
				}
			}
		}
		cells = std::make_shared<bool*>(tmp);
	}
};



template <class T> int check_cell(const T& a) {
	int alive = 0;
	for (int i = 0; i < a.size; i++) {
		if (a.cells.get()[0][i]) alive++;
	}
	if (a.if_cell_alive == true) {
		if (alive == 2 || alive == 3) return true;
		return false;
	}
	else {
		if (alive == 3) return true;
		return false;
	}
}

template<class T>
class Game {
public:
	int size;
	Board last_board;
	Board new_board;

	Game(int size) {
		this->size = size;
		last_board = create_board(size);
		new_board = create_board(size);
		initialize();
	}

	void make_iteration() {
		print();
		new_board = create_board(size);
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				T neighborhood(last_board, i, j);
				if (check_cell<decltype(neighborhood)>(neighborhood)) new_board.cells.get()[0][i][j] = true;
			}
		}
		last_board = std::move(new_board);
	}

	void print() {
		for (int i = 0; i < size; i++) {
			for (int j = 0; j < size; j++) {
				if (last_board.cells.get()[0][i][j]) std::cout << "1 ";
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
			last_board.cells.get()[0][i][j] = true;
			new_board.cells.get()[0][i][j] = true;
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

};


void move_semantics_test() {
	int x;
	Board board(6);
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells.get() << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	board = create_board(5);
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells.get() << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	Board board2(std::move(board));
	std::cout << "board" << std::endl;
	std::cout << "Object address: " << &board << "  " << "Array address: " << board.cells.get() << "  ";
	std::cout << "Size value: " << board.size << std::endl;
	std::cout << "board2" << std::endl;
	std::cout << "Object address: " << &board2 << "  " << "Array address: " << board2.cells.get() << "  ";
	std::cout << "Size value: " << board2.size << std::endl;
	std::cin >> x;
}

int main()
{
	Game<MooreNeighborhood> game(8);
	game.run(10);
	//move_semantics_test();
	system("PAUSE");
	return 0;
}
