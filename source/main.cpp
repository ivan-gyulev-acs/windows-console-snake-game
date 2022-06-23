#include "game.cpp"
#include <ctime>
#include <cmath>

auto main() -> int {
	srand(time(nullptr));
	Game game;
	game.Run();
}