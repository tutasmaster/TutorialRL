#pragma once

#include "Error.h"

#include "Map.h"

#include "Entity.h"

class Engine
{
public:

	int x, y;
	int yPosition;

	Entity player;

	Map map;

	TCOD_key_t lastKey;

	Engine();
	~Engine();

	void term();
	void init();
	void render();
	void update();
};

extern Engine engine;
