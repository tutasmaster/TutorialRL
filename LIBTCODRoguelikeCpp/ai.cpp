#include "ai.hpp"

#include "actor.hpp"
#include "engine.hpp"
#include <math.h>

void PlayerAi::update(Actor * owner)
{
	
	TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_RELEASED);

	if (looking) {
		lookUpdate(owner,key);
		return;
	}

	int mx = 0;
	int my = 0; //Movement X and Movement Y

	switch (key.vk) {
	case TCODK_UP: case TCODK_KP8:
		my -= 1;
		break;
	case TCODK_DOWN: case TCODK_KP2:
		my += 1;
		break;
	case TCODK_LEFT: case TCODK_KP4:
		mx -= 1;
		break;
	case TCODK_RIGHT: case TCODK_KP6:
		mx += 1;
		break;

	case TCODK_KP7:
		mx -= 1;
		my -= 1;
		break;

	case TCODK_KP9:
		mx += 1;
		my -= 1;
		break;

	case TCODK_KP1:
		mx -= 1;
		my += 1;
		break;

	case TCODK_KP3:
		mx += 1;
		my += 1;
		break;
	
	case TCODK_CHAR:
		if (key.c == 'i'){
			Actor* a = getItemFromInventory(owner);
			if (a != NULL && a->pickable) {
				a->pickable->use(a, owner);
			}
		}
		else if (key.c == 'g') {
			for (auto const& a : engine.actors) {
				if (a != owner && a->x == owner->x && a->y == owner->y) {
					if (a->pickable) {
						Message * msg = new Message("You pick up " + a->name + "!", TCODColor::red);
						engine.addMsg(msg);
						a->pickable->pick(a, owner);
						break;
					}
				}
			}
		}
		else if (key.c == '>') {
			if (owner->x == engine.stairs->x && owner->y == engine.stairs->y) {
				engine.nextLevel();
			}
		}
		else if (key.c == 'l') {
			look(owner,true);
		}
		else if (key.c == 'd') {
			Actor* a = getItemFromInventory(owner);
			if (a != NULL && a->pickable) {
				a->pickable->drop(a, owner);
			}
		}
		break;
	}

	move(owner,owner->x + mx,owner->y + my);

	if (mx != 0 || my != 0 || key.vk == TCODK_KP5 || key.c == '.') {
		engine.status = engine.acting;
	}
}

void PlayerAi::renderGUI(Actor * owner)
{
	if (looking) {
		TCODConsole::root->setDefaultForeground(TCODColor::lightCyan);
		TCODConsole::root->printFrame(67, 0, 13, 7);
		int i = 0;

		if (scroll < 0) scroll = 0;

		for (auto const& a : engine.actors) {
			if (a != owner && a->x == lookX && a->y == lookY){
				if (engine.map->isInFov(a->x, a->y) && i >= scroll && i <= 4 + scroll) {
					TCODConsole::root->setDefaultForeground(a->col);
					TCODConsole::root->print(68, 1 + i - scroll, a->name.c_str());
				}
				i++;
			}
			
		}
		TCODConsole::root->setDefaultForeground(TCODColor::yellow);
		TCODConsole::root->print(lookX,lookY + 7, "+");



		if (i - 5 - scroll >= 1) {
			TCODConsole::root->setDefaultForeground(TCODColor::white);
			TCODConsole::root->print(78, 6, "D");
		}

		if (scroll > 0) {
			TCODConsole::root->setDefaultForeground(TCODColor::white);
			TCODConsole::root->print(78, 0, "U");
		}

		if (i - 5 - scroll < 0) scroll = 0;
	}
}

bool PlayerAi::move(Actor * owner, int tx, int ty)
{
	bool hasWall = false;

	if (engine.map->isWall(tx, ty)) hasWall = true;

	for (auto const& a : engine.actors) {
		if (a->x == tx && a->y == ty) {
			if(a->solid && hasWall == false) hasWall = true;
			if (a->destructible) {
				std::string name = a->name;
				Message * msg;
				;
				if (owner->attacker->attack(owner, a)) {
					msg = new Message(name + " dies from your blow!", TCODColor::red);
				}
				else {
					msg = new Message("You hit " + name + "!", TCODColor::white);
				}

				engine.addMsg(msg);
				
				break;
			}
		}
	}

	if (!hasWall) {
		owner->x = tx;
		owner->y = ty;
	}

	return false;
}

void PlayerAi::look(Actor *owner, bool toggle)
{
	if (toggle == true) {
		looking = true;
		scroll = 0;
		lookX = owner->x;
		lookY = owner->y;
	}
	else {
		looking = false;
	}
}

void PlayerAi::lookUpdate(Actor *owner, TCOD_key_t key)
{
	int mx = 0;
	int my = 0;

	switch (key.vk) {
	case TCODK_UP: case TCODK_KP8:
		my -= 1;
		break;
	case TCODK_DOWN: case TCODK_KP2:
		my += 1;
		break;
	case TCODK_LEFT: case TCODK_KP4:
		mx -= 1;
		break;
	case TCODK_RIGHT: case TCODK_KP6:
		mx += 1;
		break;

	case TCODK_KP7:
		mx -= 1;
		my -= 1;
		break;

	case TCODK_KP9:
		mx += 1;
		my -= 1;
		break;

	case TCODK_KP1:
		mx -= 1;
		my += 1;
		break;

	case TCODK_KP3:
		mx += 1;
		my += 1;
		break;
	case TCODK_CHAR:
		if (key.c == 'l') {
			look(owner, false);
		}
		return;
	case TCODK_KPADD:
		scroll++;
		break;
	case TCODK_KPSUB:
		scroll--;
		break;
	}

	if(engine.map->isInFov(lookX + mx, lookY + my)){
		lookX += mx;
		lookY += my;
	}
}

Actor * PlayerAi::getItemFromInventory(Actor * owner)
{
	bool finished = false;
	int selection = 0;

	if (owner->container && owner->container->inventory.size() > 0) {
		while(true){
			for (int i = 0; i < owner->container->inventory.size(); i++)
				TCODConsole::root->print(68, 11 + i, i == selection ? ">" : " ");
			TCODConsole::flush();
			TCOD_key_t key = TCODConsole::checkForKeypress(TCOD_KEY_RELEASED);
			switch (key.vk) {
			case TCODK_KPADD:
			case TCODK_DOWN:
			case TCODK_KP2:
				selection ++;
				break;
			case TCODK_KPSUB:
			case TCODK_UP:
			case TCODK_KP8:
				selection --;
				break;
			case TCODK_ENTER:
			case TCODK_KPENTER:
				return owner->container->inventory.get(selection);
				break;
			}

			if (selection >= owner->container->inventory.size())
				selection = 0;

			if (selection < 0)
				selection = owner->container->inventory.size() - 1;

		}
	}
	else {
		return NULL;
	}

	return NULL;
	
}

void EnemyAi::update(Actor *owner) {
	if (turns < 1) {
		turns++;
	}else{
		turns = 0;
		if ((sqrt( ((engine.player->x - owner->x) * (engine.player->x - owner->x)) + ((engine.player->y - owner->y) * (engine.player->y - owner->y)))) < 10) {
			int mx = owner->x;
			int my = owner->y;
			if (engine.player->x > owner->x) mx += 1;
			else if (engine.player->x < owner->x) mx -= 1;

			if (engine.player->y > owner->y) my += 1;
			else if (engine.player->y < owner->y) my -= 1;

			move(owner, mx, my);
		}
	}
}

bool EnemyAi::move(Actor * owner, int tx, int ty)
{
	bool hasWall = false;

	if (engine.map->isWall(tx, ty)) hasWall = true;

	for (auto const& a : engine.actors) {
		if (a != owner && a->x == tx && a->y == ty) {
			if (a->solid) hasWall = true;
			if (a->destructible) {
				owner->attacker->attack(owner, a);
			}
		}
	}
	
	if (engine.player->x == tx && engine.player->y == ty) {
		owner->attacker->attack(owner, engine.player);
		hasWall = true;
	}

	if (!hasWall) {
		owner->x = tx;
		owner->y = ty;
	}

	return false;
}

void FriendAi::update(Actor *owner) {
	if (turns < 1) {
		turns++;
	}
	else {
		turns = 0;
		if ((sqrt(((engine.player->x - owner->x) * (engine.player->x - owner->x)) + ((engine.player->y - owner->y) * (engine.player->y - owner->y)))) > 15) {
			int mx = owner->x;
			int my = owner->y;
			if (engine.player->x > owner->x) mx += 1;
			else if (engine.player->x < owner->x) mx -= 1;

			if (engine.player->y > owner->y) my += 1;
			else if (engine.player->y < owner->y) my -= 1;

			move(owner, mx, my);
		}
		else {
			for (auto const& a : engine.actors) {
				
				if (a != owner && (sqrt(((a->x - owner->x) * (a->x - owner->x)) + ((a->y - owner->y) * (a->y - owner->y)))) < 10) {
					int mx = owner->x;
					int my = owner->y;
					if (a->x > owner->x) mx += 1;
					else if (a->x < owner->x) mx -= 1;

					if (a->y > owner->y) my += 1;
					else if (a->y < owner->y) my -= 1;

					move(owner, mx, my);
					break;
				}
			}

			int mx = owner->x + (rand() % 2) - 1;
			int my = owner->y + (rand() % 2) - 1;

			move(owner, mx, my);

		}
	}
}

bool FriendAi::move(Actor * owner, int tx, int ty)
{
	bool hasWall = false;

	if (engine.map->isWall(tx, ty)) hasWall = true;

	for (auto const& a : engine.actors) {
		if (a != owner && a->x == tx && a->y == ty) {
			if (a->solid) hasWall = true;
			if (a->destructible) {
				owner->attacker->attack(owner, a);
			}
		}
	}

	if (engine.player->x == tx && engine.player->y == ty) {
		owner->attacker->attack(owner, engine.player);
		hasWall = true;
	}

	if (!hasWall) {
		owner->x = tx;
		owner->y = ty;
	}

	return false;
}