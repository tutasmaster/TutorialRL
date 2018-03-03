#pragma once
#include "libtcod.hpp"
class Actor;
class Ai
{
public:
	virtual void update(Actor *owner) = 0;
	virtual void renderGUI(Actor *owner) = 0;
};

class PlayerAi : public Ai
{
	int lookX, lookY,scroll;
	bool looking = false;

	void update(Actor *owner) override;
	void renderGUI(Actor * owner) override;
	bool move(Actor *owner, int tx,int ty);

	void look(Actor *owner, bool toggle);
	void lookUpdate(Actor *owner, TCOD_key_t key);

	Actor* getItemFromInventory(Actor *owner);
};

class EnemyAi : public Ai
{
	int turns = 0;
	void update(Actor *owner) override;
	void renderGUI(Actor * owner) {};
	bool move(Actor *owner, int tx, int ty);
};

class FriendAi : public Ai
{
	int turns = 0;
	void update(Actor *owner) override;
	void renderGUI(Actor * owner) {};
	bool move(Actor *owner, int tx, int ty);
};

class ArrowAi : public Ai
{
	int turns = 0;
	void update(Actor *owner) override;
	void renderGUI(Actor * owner);
};