build: source/*
	g++ -std=c++2a -o binaries/debug/windows-console-snake-game source/main.cpp

run:
	mingw32-make.exe
	binaries/debug/windows-console-snake-game.exe
