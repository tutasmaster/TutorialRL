#include "destructible.hpp"
#include "actor.hpp"
#include "engine.hpp"
Destructible::Destructible(int maxHP = 50, int DP = 10, int XP = 10, std::string name = "ACTOR") : maxHP(maxHP), DP(DP), XP(XP), name(name)
{
	curHP = maxHP;
}

bool Destructible::damage(Actor * owner, int ammount)
{
	ammount -= DP;

	if (ammount > 0) {
		curHP -= ammount;
	}
	if (curHP <= 0) {
		curHP = 0;
		die(owner);
	}

	return (curHP <= 0);
}

void Destructible::die(Actor * owner)
{
	delete owner->ai;
	delete owner->attacker;
	owner->ai = NULL;
	owner->attacker = NULL;
	owner->name = this->name;
	owner->ch = '.';
	owner->solid = false;

	engine.actors.remove(owner);
	engine.actors.insertBefore(owner, 0);

	owner->destructible = NULL;
	delete this;
	
}

void Destructible::healing(int ammount)
{
	curHP += ammount;
	if (curHP > maxHP) curHP = maxHP;
}
