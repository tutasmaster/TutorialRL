#pragma once
#include "libtcod.h"
#include "Map.h"

#include <string>

class Ai {
public:
	virtual void OnTick() = 0;
};

class PlayerAi : public Ai {
public:
	void OnTick() override;
};

class Entity
{
public:
	Entity();
	~Entity();

	Map::Pos pos;

	Ai* ai = nullptr;

	std::string name = "Entity";
	char c = 'E';
	TCODColor col = TCODColor::yellow;
};