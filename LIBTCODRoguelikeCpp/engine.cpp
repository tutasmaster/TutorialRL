#include "engine.hpp"

Engine::Engine() : map(255,255,255)
{

	TCODConsole::initRoot(128, 64, "Roguelike");

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

	map.SetTileAt(14, 9, 1,  TILE_FLOOR);
	map.SetTileAt(14, 10, 1, TILE_FLOOR);

	map.SetTileAt(14, 9, 2,  TILE_EMPTY);
	map.SetTileAt(14, 10, 2, TILE_EMPTY);

	DrawSquareOnMap(map, 7, 7, 6, 6, 5, TILE_EMPTY);
	DrawSquareOnMap(map, 8, 8, 4, 4, 4, TILE_EMPTY);
	DrawSquareOnMap(map, 9, 9, 2, 2, 3, TILE_EMPTY);
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
					if (map.GetTileAt(i-g, j-g, z) != nullptr && map.GetTileAt(i-g, j-g, z)->type != Map::Tile::empty) {
						if (map.GetTileAt(i - g, j - g, z)->shadeLimit < g)
							break;
						TCODColor col = map.GetTileAt(i-g,j-g,z)->color;
						float a = col.getValue();
						col.setValue((a/(g+1 + (3 - ((map.GetTileAt(i - g, j - g, z)->shadeLimit)/2)))));

						TCODColor bg = map.GetTileAt(i - g, j - g, z)->bg;
						float b = bg.getValue();
						bg.setValue((b/(g+1 + (3 - ((map.GetTileAt(i - g, j - g, z)->shadeLimit) / 2)))));

						TCODConsole::root->setCharForeground(i, j, col);
						TCODConsole::root->setCharBackground(i, j, bg);
						TCODConsole::root->setChar(i, j, map.GetTileAt(i-g, j-g, z)->c);
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

	TCODConsole::root->setCharBackground(x,y,map.GetTileAt(x,y,yPosition)->bg);
	TCODConsole::root->setCharForeground(x,y,TCODColor::gold);
	TCODConsole::root->setChar(x, y, '@');
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