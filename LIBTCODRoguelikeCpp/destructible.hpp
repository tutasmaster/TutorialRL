#pragma once
#include <string>
class Actor;
class Destructible
{
public:
	int maxHP;
	int curHP;
	int DP;
	int XP;
	std::string name;
	

	Destructible(int maxHP, int DP, int XP, std::string name);

	bool damage(Actor * owner, int ammount);
	void die(Actor * owner);
	void healing(int ammount);
};

