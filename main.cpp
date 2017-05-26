#include <iostream>
#include <string>
#include <sstream>
#include <time.h>

void print_board(int board[], int size, int shots, bool debug = false) {
	std::cout << "\n  1 2 3 4 5\n\n1 ";

	for (int x = 0; x < 25; x++) {

		int cell = board[x];
		
		if (debug == false) {
			if (cell == -2) {
				std::cout << "X ";
			} else if (cell == -1) {
				std::cout << "# ";
			} else {
				std::cout << "O ";
			}
		} else {
			std::cout << cell << ' ';
		}

		if (x % 5 == 4) {
			std::cout << '\n';
			if (x < 24) {
				std::cout << (x/5)+2 << ' ';
			}
		}
	}
	std::cout << "\n" << "Shots Left: " << shots << "\n";
}

void create_board(int board[], int size) {
	for (int cell = 0; cell < 25; cell++) {
		board[cell] = 0;
	}
}

bool check_ship(int board[], int direction, int axis, int start, int end) {
	int val = 0;
	for (int location = start; location < end; location++) {
		if (direction == 0) { // This looks a lot better than a complex formula.
			val = axis+(location*5);
		} else {
			val = location+(direction*axis);
		}
		if (board[val] != 0) {
			return false;
		}
	}
	return true;
}

bool check_shot(int board[], int ships[], int size, int location) {
	int ship_id = board[location];

	for (int ship = 0; ship < size/8; ship++) {
		if (ships[ship*2] == ship_id) {
			board[location] = -2;
			ships[ship*2+1]--;
			return true;
		}
	}
	board[location] = -1;

	return false;
}

int check_alive(int ships[], int size) {
	int alive = 0;
	for (int ship = 0; ship < size/8; ship++) {
		if (ships[ship*2+1] > 0) {
			alive++;
		}
	}
	return alive;
}

void place_ship(int board[], int id, int direction, int axis, int start, int end) {
	int val = 0;
	for (int location = start; location < end; location++) {
		if (direction == 0) { // This is a lot easier to understand than a complex formula.
			val = axis+(location*5);
		} else {
			val = location+(direction*axis);
		}
		board[val] = id;
	}
}

void create_ship(int board[], int id, int length) {
	int direction;
	int axis;
	int start;
	int end;

	for (int tries = 0; tries < 100; tries++) {
		direction = rand() % 2;
		axis = rand() % 5;

		start = rand() % (5 - length);
		end = start + length;

		if (check_ship(board, direction, axis, start, end)) {
			break;
		}
	}
	place_ship(board, id, direction, axis, start, end);
}

void spawn_ships(int board[], int ships[], int size) {
	for (int ship = 0; ship < size/8; ship ++) {
		create_ship(board, ships[ship*2], ships[ship*2+1]);
	}
}

int get_input(char axis) {
	std::string input_string = "";
	int input_int = -1;
	while (true) {
		std::cout << "\nPlease enter a valid " << axis << " coordinate.\n:";
		std::cin >> input_string;
		
		std::stringstream(input_string) >> input_int;

		if ( (input_int > 0) & (input_int < 6) ) {
			break;
		}
	}
	return input_int;
}

int get_ships_alive(int ships[], int size) {
	int ships_alive = 0;
	for (int ship = 0; size/8; ship++) {
		if (ships[ship*2] > 0) {
			ships_alive++;
		}
	}
	return ships_alive;
}

void game_loop(int board[], int ships[], int size, bool DEBUG) {
	int ships_alive = size/8;
	int shots_left = 20;

	int shot_x;
	int shot_y;

	print_board(board, 25, shots_left, DEBUG);
	while (shots_left > 0) {
		shot_x = get_input('x');
		shot_y = get_input('y');
		shots_left--;

		if ( check_shot(board, ships, size, (shot_x-1)+((shot_y-1)*5) )) {
			std::cout << "\nDirect Hit!\n";
		} else {
			std::cout << "\nShot Miss!\n";
		}

		if (ships_alive != check_alive(ships, size)) {
			ships_alive = check_alive(ships, size);
			std::cout << "\nShip Sunk! " << ships_alive << " Ships Remaining\n";
		}
		
		if (ships_alive <= 0) {
			std::cout << "Congraulations you sunk all their ships\n";
			break;
		}
		if (shots_left <= 0) {
			std::cout << "Sorry you ran out of ammo\n";
			break;
		}
		print_board(board, 25, shots_left, DEBUG);
	}
}

int main() {
	srand(time(NULL)); //Seed for RNG
	bool DEBUG = false;

	int board[25] = {0};
	int ships[] = {1, 2, 2, 3, 3, 4}; // Id, Size
	int ships_size = sizeof(ships);

	create_board(board, 25);
	spawn_ships(board, ships, ships_size);

	game_loop(board, ships, ships_size, DEBUG);

	system("pause");
	return 0;
}
