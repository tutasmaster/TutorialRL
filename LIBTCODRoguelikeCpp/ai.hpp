#pragma once
#include "libtcod.hpp"
class Actor;
class Ai
{
public:
	virtual void update(Actor *owner) = 0;
};

class PlayerAi : public Ai
{
	void update(Actor *owner) override;
	bool move(Actor *owner, int tx,int ty);

	Actor* getItemFromInventory(Actor *owner);
};

class EnemyAi : public Ai
{
	int turns = 0;
	void update(Actor *owner) override;
	bool move(Actor *owner, int tx, int ty);
};

class FriendAi : public Ai
{
	int turns = 0;
	void update(Actor *owner) override;
	bool move(Actor *owner, int tx, int ty);
};