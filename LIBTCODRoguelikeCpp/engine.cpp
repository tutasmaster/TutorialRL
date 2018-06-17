#include "engine.hpp"


Engine::Engine()
{
	TCODConsole::initRoot(16, 16, "Roguelike");
	
	yPosition = 1;

	map = new int[16*16*16];

	for (int i = 0; i < 16 * 16 * 16; i++) {
		map[i] = 0;
	}

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			map[(i) + (j * 16)] = 1;
		}
	}

	for (int i = 1; i < 15; i++) {
		for (int j = 1; j < 15; j++) {
			map[(i) + (j * 16) + (1 * 256)] = 1;
		}
	}

	for (int i = 2; i < 14; i++) {
		for (int j = 2; j < 14; j++) {
			map[(i)+(j * 16) + (2 * 256)] = 1;
		}
	}

	for (int i = 3; i < 13; i++) {
		for (int j = 3; j < 13; j++) {
			map[(i)+(j * 16) + (3 * 256)] = 1;
		}
	}

	for (int i = 4; i < 12; i++) {
		for (int j = 4; j < 12; j++) {
			map[(i)+(j * 16) + (4 * 256)] = 1;
		}
	}

	for (int i = 5; i < 11; i++) {
		for (int j = 5; j < 11; j++) {
			map[(i)+(j * 16) + (5 * 256)] = 1;
		}
	}

	for (int i = 6; i < 10; i++) {
		for (int j = 6; j < 10; j++) {
			map[(i)+(j * 16) + (6 * 256)] = 1;
		}
	}

	for (int i = 7; i < 9; i++) {
		for (int j = 7; j < 9; j++) {
			map[(i)+(j * 16) + (7 * 256)] = 1;
		}
	}

	for (int i = 0; i < 16; i++) {
		map[(i)+(8 * 16) + (2 * 256)] = 0;

		map[(i)+(8 * 16) + (7 * 256)] = 1;
	}
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


	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			int value = map[(i) + (j * 16) + (yPosition * 256)];
			int topValue = map[(i)+(j * 16) + ((yPosition + 1) * 256)];

			TCODConsole::root->setDefaultForeground(TCODColor::grey);

			if (value == 0) {
				TCODConsole::root->setChar(i, j, ' ');
			}
			else if(topValue == 0 && value == 1){
				TCODConsole::root->setChar(i, j, '#');
			}
			else if(value == 1){
				TCODConsole::root->setChar(i, j, '.');
			}
		}
	}

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
		y--;
	}
	else if (lastKey.vk == TCODK_DOWN) {
		y++;
	}

	if (lastKey.vk == TCODK_LEFT) {
		x--;
	}
	else if (lastKey.vk == TCODK_RIGHT) {
		x++;
	}

	int value = map[(x)+(y * 16) + ((yPosition+1) * 256)];

	if (value == 1) {
		yPosition++;
	}
	else if (value == 0) {
		int bottomValue = map[(x)+(y * 16) + ((yPosition) * 256)];

		while (bottomValue == 0) {
			yPosition--;
			bottomValue = map[(x)+(y * 16) + ((yPosition) * 256)];
		}
	}


}