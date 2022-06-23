#pragma once

#include <array>
using std::array;
#include <deque>
using std::deque;
#include "food.cpp"
#include "field.cpp"

enum VELOCITY {
	NONE,
	POSITIVE,
	NEGATIVE
};

struct Snake {
	array<size_t, 2> position = {0, 0};
	COLOR color;
	array<VELOCITY, 2> velocity = {NONE, NONE};
	deque<array<size_t, 2>> tail = {{0, 0}, {0, 0}};
	unsigned score = 0, health = 5;

	auto Update(array<size_t, 2> field_size, char command) -> void {
		if (command == 'W' && velocity[1] == NONE) velocity = {NONE, NEGATIVE};
		if (command == 'A' && velocity[0] == NONE) velocity = {NEGATIVE, NONE};
		if (command == 'S' && velocity[1] == NONE) velocity = {NONE, POSITIVE};
		if (command == 'D' && velocity[0] == NONE) velocity = {POSITIVE, NONE};

		tail.pop_front();
		tail.push_back(position);

		switch (velocity[0]) {
			case POSITIVE : {
				position[0] = (position[0] + 1) % field_size[0];
				break;
			}
			case NEGATIVE : {
				position[0] = (position[0] + field_size[0] - 1) % field_size[0];
				break;
			}
		}

		switch (velocity[1]) {
			case POSITIVE : {
				position[1] = (position[1] + 1) % field_size[1];
				break;
			}
			case NEGATIVE : {
				position[1] = (position[1] + field_size[1] - 1) % field_size[1];
				break;
			}
		}
	}
	auto DrawOnBuffer(Field & field) -> void {
		field[position] = color;
		for (auto iterator = tail.begin() + 1; iterator != tail.end(); iterator++) {
			field[*iterator] = color;
		}
	}
	auto Consume(Food & food) -> void {
		score += food.points;
		tail.push_front(tail.front());
		tail.push_front(tail.front());
		tail.push_front(tail.front());
		tail.push_front(tail.front());
		tail.push_front(tail.front());
		tail.push_front(tail.front());
	}
	auto Damage(Trap & trap) -> bool {
		score /= 2;
		tail.pop_front();
		return tail.front() == tail.back();
	}
};