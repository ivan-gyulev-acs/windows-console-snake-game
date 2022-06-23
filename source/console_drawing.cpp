#pragma once
#include <windows.h>

enum COLOR {
	BLACK = 0,
	BLUE = FOREGROUND_BLUE,
	GREEN = FOREGROUND_GREEN,
	CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
	RED = FOREGROUND_RED,
	PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
	BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
	LIGHT_GREY =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,
	GREY = 0 | FOREGROUND_INTENSITY,
	LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
	LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
	PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	YELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
	WHITE =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};

HANDLE consoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

auto DrawConsoleSquare(
	unsigned x, unsigned y,
	char symbol,
	COLOR foreground_color,
	COLOR background_color
) -> void {
	CHAR_INFO character_info;
	character_info.Char.AsciiChar = symbol;
	character_info.Attributes = foreground_color | (background_color << 4);

	SMALL_RECT screen_position = {(short)x, (short)y, (short)(x + 1), (short)(y + 1)};
	::WriteConsoleOutput(
		consoleOutput,
		&character_info,
		{1, 1},
		{0, 0},
		&screen_position
	);
}

auto DrawConsoleText(
	string text,
	unsigned x_offset = 0,
	unsigned y_offset = 0,
	COLOR foreground_color = WHITE,
	COLOR background_color = BLACK
) -> void {
	unsigned x = 0, y = 0;
	for (auto character : text) {
		if (character == '\n') {
			y += 1;
			x = 0;
			continue;
		}
		DrawConsoleSquare(x_offset + x, y_offset + y, character, foreground_color, background_color);
		x += 1;
	}
}