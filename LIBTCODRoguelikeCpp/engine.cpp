#include "engine.hpp"

#include "MapModifiers.h"
#include "Error.h"

#define Tile Map::Tile

Engine::Engine() : map(255,255,255)
{
	TCODConsole::initRoot(128, 64, "Roguelike");

	Tile tile;
	tile.type = tile.walkable;
	tile.c = '.';
	tile.color = TCODColor::white;
	tile.bg = TCODColor::black;
	tile.shadeLimit = 2;
	SetMapLayer(map, 1, tile);
	
	tile.type = tile.wall;
	tile.c = ' ';
	tile.color = TCODColor::white;
	tile.bg = TCODColor::white;

	tile.shadeLimit = 5;

	DrawSquareOnMap(map, 5, 5, 10, 10, 1, tile);
	DrawSquareOnMap(map, 5, 5, 10, 10, 2, tile);
	DrawSquareOnMap(map, 5, 5, 10, 10, 3, tile);
	DrawSquareOnMap(map, 5, 5, 10, 10, 4, tile);
	DrawSquareOnMap(map, 5, 5, 10, 10, 5, tile);

	tile.type = tile.walkable;
	tile.c = '.';
	tile.color = TCODColor::white;
	tile.bg = TCODColor::black;
	tile.shadeLimit = 2;

	DrawSquareOnMap(map, 6, 6, 8, 8, 1, tile);
	DrawSquareOnMap(map, 6, 6, 8, 8, 2, tile);
	DrawSquareOnMap(map, 6, 6, 8, 8, 3, tile);
	DrawSquareOnMap(map, 6, 6, 8, 8, 4, tile);
	DrawSquareOnMap(map, 6, 6, 8, 8, 5, tile);

	map.SetTileAt(14, 9, 1, tile);
	map.SetTileAt(14, 10, 1, tile);

	tile.type = tile.empty;
	map.SetTileAt(14, 9, 2, tile);
	map.SetTileAt(14, 10, 2, tile);

	tile.shadeLimit = 5;

	tile.type = tile.walkable;
	tile.c = '>';
	tile.color = TCODColor::white;
	tile.bg = TCODColor::black;

	map.SetTileAt(13, 9, 1, tile);
	map.SetTileAt(13, 9, 2, tile);
	map.SetTileAt(13, 9, 3, tile);
	map.SetTileAt(13, 9, 4, tile);
	tile.c = '<';

	map.SetTileAt(13, 9, 5, tile);

	tile.type = tile.empty;
	tile.shadeLimit = 2;
	DrawSquareOnMap(map, 7, 7, 6, 6, 5, tile);
	DrawSquareOnMap(map, 8, 8, 4, 4, 4, tile);
	DrawSquareOnMap(map, 9, 9, 2, 2, 3, tile);
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


	for (int j = 1; j < map.height; j++) {
		for (int i = 1; i < map.width; i++) {
			Tile* r = map.GetTileAt(i, j, yPosition + 1);

			if(r != nullptr && r->type != Map::Tile::empty){
				TCODConsole::root->setCharBackground(i - 1, j - 1, r->bg);
				TCODConsole::root->setCharForeground(i-1,j-1,r->color);
				TCODConsole::root->setChar(i-1, j-1, r->c);
			}
			else if(r != nullptr){
				int g = 0;
				for (int z = yPosition + 1; z > -1; --z) {
					if (map.GetTileAt(i-g, j-g, z) != nullptr && map.GetTileAt(i-g, j-g, z)->type != Tile::empty) {
						if (map.GetTileAt(i - g, j - g, z)->shadeLimit < g)
							break;
						TCODColor col = map.GetTileAt(i-g,j-g,z)->color;
						float a = col.getValue();
						col.setValue((a/(g+1)));

						TCODColor bg = map.GetTileAt(i - g, j - g, z)->bg;
						float b = bg.getValue();
						bg.setValue((b/(g+1)));

						TCODConsole::root->setCharForeground(i - 1, j - 1, col);
						TCODConsole::root->setCharBackground(i - 1, j - 1, bg);
						TCODConsole::root->setChar(i - 1, j - 1, map.GetTileAt(i-g, j-g, z)->c);
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

	TCODConsole::root->setCharBackground(x,y,map.GetTileAt(x+1,y+1,yPosition+1)->bg);
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
		if (map.GetTileAt(x + 1, y, yPosition + 1) != nullptr && map.GetTileAt(x + 1, y, yPosition + 1)->type == Map::Tile::walkable)
			y--;
	}
	else if (lastKey.vk == TCODK_DOWN) {
		if (map.GetTileAt(x + 1, y + 2, yPosition + 1) != nullptr && map.GetTileAt(x + 1, y + 2, yPosition + 1)->type == Map::Tile::walkable)
			y++;
	}

	if (lastKey.vk == TCODK_LEFT) {
		if(map.GetTileAt(x, y + 1, yPosition + 1) != nullptr && map.GetTileAt(x , y + 1, yPosition + 1)->type == Map::Tile::walkable)
			x--;
	}
	else if (lastKey.vk == TCODK_RIGHT) {
		if (map.GetTileAt(x + 2, y + 1, yPosition + 1) != nullptr && map.GetTileAt(x + 2, y + 1, yPosition + 1)->type == Map::Tile::walkable)
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