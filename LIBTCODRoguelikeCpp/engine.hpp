#pragma once

#include <libtcod.hpp>
#include <vector>

class Engine
{
public:

	int x, y;
	int yPosition;

	int *map;

	TCOD_key_t lastKey;

	Engine();
	~Engine();

	void term();
	void init();
	void render();
	void update();
};

extern Engine engine;
