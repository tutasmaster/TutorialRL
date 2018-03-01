#include "attacker.hpp"

#include "actor.hpp"

Attacker::Attacker(int AP) : AP(AP)
{

}

bool Attacker::attack(Actor *owner, Actor *target)
{
	if (target->destructible) {
		return target->destructible->damage(target,AP);
	}
	return false;
}


