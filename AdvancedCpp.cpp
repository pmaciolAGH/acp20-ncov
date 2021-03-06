// AdvancedCpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <fstream>


struct cell {
	bool current_state, last_state;
	cell() {
		current_state = false;
		last_state = false;
	}
};

void make_iteration(cell**);
void check_element(int, int, cell**);
void process(cell**);
void initialize_board(cell**);
void print(cell**);

int board_size = 5;
int number_of_iterations = 100;

int main()
{
	int x;
	cell** board = new cell*[board_size];
	for (int i = 0; i < board_size; i++) {
		board[i] = new cell[board_size];
	}
	initialize_board(board);
	for (int i = 0; i < number_of_iterations; i++) {
		std::cout << "iteration " << i << std::endl;
		make_iteration(board);
		Sleep(1000);
		system("CLS");
	}
	return 0;
}

void make_iteration(cell** board) {
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

void initialize_board(cell** board) {
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