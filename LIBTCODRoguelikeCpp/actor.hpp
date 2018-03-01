#pragma once
#include "libtcod.hpp"
#include <string>
#include "ai.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "container.hpp"

class Actor
{
public:
	int x, y;
	char ch;
	bool solid;
	bool fovOnly;
	TCODColor col;
	std::string name;
	

	Actor(int x, int y, bool solid, char ch, std::string name, const TCODColor & col);
	~Actor();

	Ai *ai; //Does the actor self-updates?
	Destructible *destructible; //Can the actor be killed?
	Attacker *attacker; //Can the actor damage people?
	Container *container; //Can the actor carry other actors?
	Pickable *pickable; //Can the actor be used?

	void update();
	void render();
};

