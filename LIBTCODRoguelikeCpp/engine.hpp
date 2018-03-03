#pragma once

#include <libtcod.hpp>
#include <vector>
#include "map.hpp"
#include "actor.hpp"

struct Message {
	std::string text;
	TCODColor col;
	Message(std::string text, const TCODColor &col);
	~Message();
};



class Engine
{
public:

	TCOD_key_t lastKey;

	enum EngineStatus {
		menu,
		startup,
		idle,
		acting,
		victory,
		death
	} status;

	Engine();
	~Engine();

	void init();
	void render();
	void update();
	void nextLevel();
	void addMsg(Message * text);

	int corpses = 0;
	int fovRadius = 10;
	int level = 0;

	TCODConsole con;

	Map *map;
	Actor *player;
	Actor *stairs;

	TCODList<Actor *> actors;
	TCODList<Message *> log;
};

extern Engine engine;
