#pragma once

#include <array>
using std::array;
#include <vector>
using std::vector;
#include "console_drawing.cpp"

struct Field {
	vector<COLOR> buffer;
	array<size_t, 2> size;
	COLOR color;
	
	Field(array<size_t, 2> size, COLOR color):
		buffer(),
		size(size),
		color(color)
	{
		buffer.reserve(size[0] * size[1]);
		for (auto _ = 0; _ < size[0] * size[1]; _ += 1)
			buffer.push_back(color);
	}
	auto operator[](array<size_t, 2> position) -> COLOR & {
		return buffer[position[1] * size[0] + position[0]];
	}
	auto FlushBuffer(array<size_t, 2> offset = {0, 0}) -> void {
		for (size_t y = 0; y < size[1]; y += 1) {
			for (size_t x = 0; x < size[0] * 2; x += 1) {
				DrawConsoleSquare(
					offset[0] + x,
					offset[1] + y,
					' ',
					(*this)[{x / 2, y}],
					(*this)[{x / 2, y}]
				);
			}
		}
	}
	auto ResetBuffer() -> void {
		for (auto & item : buffer) item = color;
	}
};