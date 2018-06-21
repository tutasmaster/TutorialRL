#include "engine.hpp"

Engine::Engine() : map(255,255,255)
{

	TCODConsole::initRoot(128, 64, "Roguelike");

	PlayerAi p;
	player.ai = &p;

	SetMapLayer(map, 1, TILE_FLOOR);
	
	DrawSquareOnMap(map, 5, 5, 10, 10, 1, TILE_WALL);
	DrawSquareOnMap(map, 5, 5, 10, 10, 2, TILE_WALL);
	DrawSquareOnMap(map, 5, 5, 10, 10, 3, TILE_WALL);
	DrawSquareOnMap(map, 5, 5, 10, 10, 4, TILE_WALL);
	DrawSquareOnMap(map, 5, 5, 10, 10, 5, TILE_WALL);

	DrawSquareOnMap(map, 6, 6, 8, 8, 1, TILE_FLOOR);
	DrawSquareOnMap(map, 6, 6, 8, 8, 2, TILE_FLOOR);
	DrawSquareOnMap(map, 6, 6, 8, 8, 3, TILE_FLOOR);
	DrawSquareOnMap(map, 6, 6, 8, 8, 4, TILE_FLOOR);
	DrawSquareOnMap(map, 6, 6, 8, 8, 5, TILE_FLOOR);

	DrawSquareOnMap(map, 20, 5, 10, 10, 1, TILE_WALL);
	DrawSquareOnMap(map, 20, 5, 10, 10, 2, TILE_WALL);
	DrawSquareOnMap(map, 20, 5, 10, 10, 3, TILE_WALL);
	DrawSquareOnMap(map, 20, 5, 10, 10, 4, TILE_WALL);
	DrawSquareOnMap(map, 20, 5, 10, 10, 5, TILE_WALL);

	DrawSquareOnMap(map, 21, 6, 8, 8, 1, TILE_FLOOR);
	DrawSquareOnMap(map, 21, 6, 8, 8, 2, TILE_FLOOR);
	DrawSquareOnMap(map, 21, 6, 8, 8, 3, TILE_FLOOR);
	DrawSquareOnMap(map, 21, 6, 8, 8, 4, TILE_FLOOR);
	DrawSquareOnMap(map, 21, 6, 8, 8, 5, TILE_FLOOR);

	DrawSquareOnMap(map, 14, 8, 7, 4, 3, TILE_WALL);
	DrawSquareOnMap(map, 14, 8, 2, 4, 4, TILE_WALL);
	DrawSquareOnMap(map, 14, 9, 7, 2, 4, TILE_FLOOR);

	DrawSquareOnMap(map, 21, 6, 8, 8, 6, TILE_WALL);
	DrawSquareOnMap(map, 22, 7, 6, 6, 7, TILE_WALL);
	DrawSquareOnMap(map, 23, 8, 4, 4, 8, TILE_WALL);
	DrawSquareOnMap(map, 24, 9, 2, 2, 9, TILE_WALL);

	DrawSquareOnMap(map, 22, 7, 6, 6, 6, TILE_FLOOR);
	DrawSquareOnMap(map, 23, 8, 4, 4, 7, TILE_FLOOR);
	DrawSquareOnMap(map, 24, 9, 2, 2, 8, TILE_FLOOR);

	map.SetTileAt(14, 9, 1,  TILE_FLOOR);
	map.SetTileAt(14, 10, 1, TILE_FLOOR);

	map.SetTileAt(14, 9, 2,  TILE_EMPTY);
	map.SetTileAt(14, 10, 2, TILE_EMPTY);

	map.SetTileAt(20, 9, 1, TILE_FLOOR);
	map.SetTileAt(20, 10, 1, TILE_FLOOR);

	map.SetTileAt(20, 9, 2, TILE_EMPTY);
	map.SetTileAt(20, 10, 2, TILE_EMPTY);
}




Engine::~Engine()
{

}

void Engine::term()
{

}

void Engine::init()
{

}

void Engine::render()
{
	TCODConsole::root->clear();


	for (int j = 0; j < map.height; j++) {
		for (int i = 0; i < map.width; i++) {
			Map::Tile* r = map.GetTileAt(i,j,yPosition);

			if(r != nullptr && r->type != r->empty){
				TCODConsole::root->setCharBackground(i, j, r->bg);
				TCODConsole::root->setCharForeground(i,j,r->color);
				TCODConsole::root->setChar(i, j, r->c);
			}
			else if(r != nullptr){
				int g = 0;
				for (int z = yPosition; z > -1; --z) {
					Map::Tile * t = map.GetTileAt(i - g, j - g, z);
					int temp = g;
					if (g != 0 && map.GetTileAt((i - g) + 1, (j - g) + 1, z) != nullptr && map.GetTileAt((i - g) + 1, (j - g) + 1, z)->type == Map::Tile::wall){
						t = map.GetTileAt((i - g) + 1, (j - g) + 1, z);
						temp--;
					}
					if (t != nullptr && t->type != Map::Tile::empty) {
						if (t->shadeLimit < g)
							break;

						TCODColor col = t->color;
						float a = col.getValue();
						col.setValue((a/(temp+1 + (3 - ((t->shadeLimit)/2)))));


						TCODColor bg = t->bg;
						float b = bg.getValue();
						bg.setValue((b/(temp+1 + (3 - ((t->shadeLimit) / 2)))));

						

						TCODConsole::root->setCharForeground(i, j, col);
						TCODConsole::root->setCharBackground(i, j, bg);
						TCODConsole::root->setChar(i, j, t->c);
						break;
					}

					g++;

					if (g > 5)
						break;
				}
			}
			else {
				Err::ConsoleLog("Something awfull happened while drawing the map... run!");
			}
		}
	}

	TCODConsole::root->setCharBackground(player.pos.w, player.pos.h,map.GetTileAt(x,y,yPosition)->bg);
	TCODConsole::root->setCharForeground(player.pos.w, player.pos.h,TCODColor::gold);
	TCODConsole::root->setChar(player.pos.w, player.pos.h, '@');
}

void Engine::update()
{
	lastKey = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);

	if (lastKey.c == '<') {
		if (yPosition != 0)
			yPosition--;
	}
	else if (lastKey.c == '>') {
		yPosition++;
	}

	if (lastKey.vk == TCODK_UP) {
		if (map.GetTileAt(x, y - 1, yPosition) != nullptr && map.GetTileAt(x, y - 1, yPosition)->type == Map::Tile::floor)
			y--;
	}
	else if (lastKey.vk == TCODK_DOWN) {
		if (map.GetTileAt(x, y + 1, yPosition) != nullptr && map.GetTileAt(x, y + 1, yPosition)->type == Map::Tile::floor)
			y++;
	}

	if (lastKey.vk == TCODK_LEFT) {
		if(map.GetTileAt(x - 1, y, yPosition) != nullptr && map.GetTileAt(x - 1, y, yPosition)->type == Map::Tile::floor)
			x--;
	}
	else if (lastKey.vk == TCODK_RIGHT) {
		if (map.GetTileAt(x + 1, y, yPosition) != nullptr && map.GetTileAt(x + 1, y, yPosition)->type == Map::Tile::floor)
			x++;
	}

	/*Tile::Type value = map.GetTileAt(x,y,yPosition)->type;

	if (value == Tile::Type::wall) {
		yPosition++;
	}
	else if (value == 0) {
		Tile::Type bottomValue = map.GetTileAt(x, y, yPosition-1)->type;

		while (bottomValue == 0) {
			yPosition--;
			bottomValue = map.GetTileAt(x,y,yPosition)->type;
		}
	}*/


}