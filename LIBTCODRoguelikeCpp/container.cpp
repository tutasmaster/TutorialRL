#include "container.hpp"

#include "engine.hpp"

#include "actor.hpp"

Container::Container(int size) : size(size)
{
}


Container::~Container()
{
	inventory.clearAndDelete();
}

bool Container::add(Actor *actor) 
{
	if (size > 0 && inventory.size() >= size) {
		return false;
	}
	inventory.push(actor);
	return true;
}

void Container::remove(Actor *actor)
{
	inventory.remove(actor);
}



bool Pickable::pick(Actor * owner, Actor * wearer)
{
	if (wearer->container && wearer->container->add(owner)) {
		engine.actors.remove(owner);
		return true;
	}
	return false;
}

void Pickable::drop(Actor *owner, Actor *wearer) {
	if (wearer->container) {
		wearer->container->remove(owner);
		engine.actors.push(owner);
		owner->x = wearer->x;
		owner->y = wearer->y;
	}
}

bool Pickable::use(Actor *owner, Actor *wearer) {
	if (wearer->container) {
		wearer->destructible->healing(100);
		wearer->container->remove(owner);
		delete owner;
		return true;
	}
	return false;
}
