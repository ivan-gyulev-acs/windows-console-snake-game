#pragma once

#include <array>
#include "field.cpp"

struct Food {
	std::array<size_t, 2> position = {0, 0};
	COLOR color;
	int points = 0;
	auto DrawOnBuffer(Field & field) -> void {
		field[position] = color;
	}
};