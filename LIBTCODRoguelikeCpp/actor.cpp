#include "actor.hpp"
#include "engine.hpp"

Actor::Actor(int x, int y, bool solid, char ch, const std::string name, const TCODColor & col) : x(x), y(y), solid(solid), ch(ch), col(col), name(name), ai(NULL), destructible(NULL), attacker(NULL), fovOnly(true)
{
}


Actor::~Actor()
{
	if (ai) delete ai;
	if (attacker) delete attacker;
	if (destructible) delete destructible;
}

void Actor::update()
{
	if (ai) ai->update(this);
}

void Actor::render()
{
	engine.con.setChar(x, y, ch);
	engine.con.setCharForeground(x, y, col);
}