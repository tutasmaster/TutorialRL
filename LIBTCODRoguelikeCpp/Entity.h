#pragma once
#include "libtcod.h"
#include "Map.h"

#include <string>
#include <memory>

class Entity;

class Ai {
public:
	virtual void OnTick(std::shared_ptr<Entity> entity) = 0;
};

class PlayerAi : public Ai {
public:
	void OnTick(std::shared_ptr<Entity> entity) override;
};

class Entity
{
public:
	Entity();

	Map::Pos pos;

	std::shared_ptr<Ai> ai;

	std::string name = "Entity";
	char c = 'E';
	TCODColor col = TCODColor::yellow;
};