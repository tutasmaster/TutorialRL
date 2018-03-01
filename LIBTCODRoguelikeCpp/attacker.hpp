#pragma once

class Actor;

class Attacker
{
public:
	int AP;
	Attacker(int AP);

	bool attack(Actor * owner, Actor * target);
};

