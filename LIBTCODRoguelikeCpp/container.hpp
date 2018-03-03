#pragma once
#include "libtcod.hpp"

class Actor;

class Container
{
public:

	int size;

	Container(int size);
	~Container();

	TCODList <Actor *> inventory;

	bool add(Actor *actor);
	void remove(Actor *actor);
};

class Pickable
{
public:
	bool pick(Actor *owner, Actor *wearer);
	void drop(Actor *owner, Actor *wearer);
	virtual bool use(Actor *owner, Actor *wearer);
};

class HealthPickable : public Pickable
{
	bool use(Actor *owner, Actor *wearer) override;
};

class BowPickable : public Pickable
{
	bool use(Actor *owner, Actor *wearer) override;
};