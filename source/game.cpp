#pragma once

#include <cmath>
#include <array>
using std::array;
#include <vector>
using std::vector;
#include <string>
using std::string;

#include "food.cpp"
#include "trap.cpp"
#include "snake.cpp"

const array<size_t, 2> FIELD_SIZE = {50, 50};
const COLOR FIELD_COLOR = GREEN;
const COLOR SNAKE_COLOR = BLUE;
const COLOR FOOD_COLOR = RED;
const COLOR TRAP_COLOR = YELLOW;

class Game {
	Field field;
	Snake snake;
	vector<Food> foods;
	vector<Trap> traps;
	bool is_game_over;

public:
	Game(): field(FIELD_SIZE, FIELD_COLOR) {}
	virtual ~Game() {
		::SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
		);
		::FlushConsoleInputBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
		system("cls");
	}
	auto Run() -> void {
		while (true) {
			Initialize();
			system("cls");
			ShowInstructions();
			FirstAction();
			while (true) {
				if (GetAsyncKeyState(VK_ESCAPE)) return;
				Sleep(1000 / 120);
				Update();
				if (is_game_over) break;
				Display({0, 3});
			}
			if (EndSequence()) return;
		}
	}
private:
	auto RandomEmptyPosition() -> array<size_t, 2> {
		array<size_t, 2> random_position =
			{rand() % field.size[0], rand() % field.size[1]};
		while (this->field[random_position] != FIELD_COLOR) {
			random_position[0] += 1;
			random_position[1] += random_position[0] / field.size[0];
			random_position[0] %= field.size[0];
		}
		return random_position;
	}
	auto ShowInstructions() -> void {
		DrawConsoleText("press ESCAPE to exit, move with [wasd]");
		string entity_appereance = "Trap: --, Food: ++";
		DrawConsoleText(entity_appereance, 0, 1);
		DrawConsoleSquare(entity_appereance.find('+') + 0, 1, ' ', FOOD_COLOR, FOOD_COLOR);
		DrawConsoleSquare(entity_appereance.find('+') + 1, 1, ' ', FOOD_COLOR, FOOD_COLOR);
		DrawConsoleSquare(entity_appereance.find('-') + 0, 1, ' ', TRAP_COLOR, TRAP_COLOR);
		DrawConsoleSquare(entity_appereance.find('-') + 1, 1, ' ', TRAP_COLOR, TRAP_COLOR);
	}
	auto FirstAction() -> void {
		Display({0, 3});
		while (true) {
			if (GetAsyncKeyState(VK_ESCAPE)) return;
			if (GetAsyncKeyState('W')) snake.Update(field.size, 'W');
			else if	(GetAsyncKeyState('A')) snake.Update(field.size, 'A');
			else if	(GetAsyncKeyState('S')) snake.Update(field.size, 'S');
			else if	(GetAsyncKeyState('D')) snake.Update(field.size, 'D');
			else continue;
			break;
		}	
	}
	auto EndSequence() -> bool {
		string end_message[2] = {
			"GAME OVER!",
			"press ESCAPE to exit or SPACE to start a new game"
		};
		DrawConsoleText(
			end_message[0],
			(FIELD_SIZE[0] * 2 - end_message[0].length()) / 2,
			2 + FIELD_SIZE[1] / 2,
			BLACK, WHITE
		);
		DrawConsoleText(
			end_message[1],
			(FIELD_SIZE[0] * 2 - end_message[1].length()) / 2,
			2 + FIELD_SIZE[1] / 2 + 1,
			BLACK, WHITE
		);
		while (!GetAsyncKeyState(VK_SPACE))	{
			if (GetAsyncKeyState(VK_ESCAPE)) return true;
		}
		return false;
	}
	auto Initialize() -> void {
		field.ResetBuffer();
		snake = Snake {.color = SNAKE_COLOR};
		foods = vector<Food>(5, Food {.color = FOOD_COLOR});
		traps = vector<Trap>(10 + rand() % 20, Trap {.color = TRAP_COLOR});
		is_game_over = false;
		snake.DrawOnBuffer(field);
		for (auto & food : foods) {
			food.position = RandomEmptyPosition();
			food.points = 5 + rand() % 10;
			food.DrawOnBuffer(field);
		}
		for (auto & trap : traps) {
			trap.position = RandomEmptyPosition();
			trap.DrawOnBuffer(field);
		}
		field.ResetBuffer();
	}
	auto Update() -> void {
		auto command = ' ';
		if (GetAsyncKeyState('W')) command = 'W';
		else if	(GetAsyncKeyState('S')) command = 'S';
		else if	(GetAsyncKeyState('D')) command = 'D';
		else if	(GetAsyncKeyState('A')) command = 'A';
		snake.Update(field.size, command);
		for (auto iterator = traps.begin(); iterator != traps.end(); iterator++) {
			if (snake.position != iterator->position) continue;
			if (snake.Damage(*iterator)) {
				is_game_over = true;
				return;
			}
			traps.erase(iterator);
			return;
		}
		for (auto & food : foods) {
			if (snake.position != food.position) continue;
			snake.Consume(food);
			food.position = RandomEmptyPosition();
			return;
		}
		if (field[snake.position] == SNAKE_COLOR) is_game_over = true;
	}
	auto Display(array<size_t, 2> offset) -> void {
		DrawConsoleText("score: " + std::to_string(snake.score), 0, 2);
		field.ResetBuffer();
		for (auto food : foods) food.DrawOnBuffer(field);
		for (auto trap : traps) trap.DrawOnBuffer(field);
		snake.DrawOnBuffer(field);
		field.FlushBuffer(offset);
	}
};