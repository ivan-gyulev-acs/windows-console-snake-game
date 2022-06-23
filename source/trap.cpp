#pragma once

#include <array>
#include "field.cpp"

struct Trap {
	std::array<size_t, 2> position = {0, 0};
	COLOR color;
	auto DrawOnBuffer(Field & field) -> void {
		field[position] = color;
	}
};