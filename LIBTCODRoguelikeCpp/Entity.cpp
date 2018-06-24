#include "Entity.h"



Entity::Entity()
{

}

void PlayerAi::OnTick(std::shared_ptr<Entity> entity)
{
	auto lastKey = TCODConsole::root->checkForKeypress(TCOD_KEY_RELEASED);

	switch (lastKey.vk) {
	case TCODK_UP:
		entity->pos.h--;
		break;
	case TCODK_DOWN:
		entity->pos.h++;
		break;
	case TCODK_LEFT:
		entity->pos.w--;
		break;
	case TCODK_RIGHT:
		entity->pos.w++;
		break;
	}
}
