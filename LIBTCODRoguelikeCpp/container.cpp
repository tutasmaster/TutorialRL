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
	return false;
}

bool BowPickable::use(Actor * owner, Actor * wearer)
{
	Actor * arrow = NULL;
	bool hasArrows = false;
	for (auto const& a : wearer->container->inventory){
		if (a->name == "Arrow") {
			arrow = a;
			hasArrows = true;
			break;
		}
	}

	if (!hasArrows) {
		Message * msg = new Message("You need arrows in order to shoot!",TCODColor::white);
		return false;
	}


	TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_RELEASED);
		while (!(key.vk == TCODK_UP || key.vk == TCODK_DOWN || key.vk == TCODK_LEFT || key.vk == TCODK_RIGHT)) {
			key = TCODConsole::checkForKeypress(TCOD_KEY_RELEASED);
			if (key.vk == TCODK_UP) {
				arrow->ch = '|';
				arrow->name = "u";
				arrow->y--;
				drop(arrow, wearer);
			}
			if (key.vk == TCODK_DOWN){
				arrow->ch = '|';
				arrow->name = "d";
				arrow->y++;
				drop(arrow, wearer);
			}
			if (key.vk == TCODK_LEFT){
				arrow->ch = '-';
				arrow->name = "l";
				arrow->x--;
				drop(arrow, wearer);
			}
			if (key.vk == TCODK_RIGHT){
				arrow->ch = '-';
				arrow->name = "r";
				arrow->x++;
				drop(arrow, wearer);
			}
	}
	return true;
}

bool HealthPickable::use(Actor * owner, Actor * wearer)
{
	if (wearer->container) {
		wearer->destructible->healing(50);
		wearer->container->remove(owner);
		delete owner;
		return true;
	}
	return false;
}
