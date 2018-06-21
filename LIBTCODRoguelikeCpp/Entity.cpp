#include "Entity.h"



Entity::Entity()
{
}


Entity::~Entity()
{
	delete ai;
	ai = nullptr;
}

void PlayerAi::OnTick()
{
}
