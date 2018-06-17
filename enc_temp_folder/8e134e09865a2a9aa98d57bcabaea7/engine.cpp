#include "engine.hpp"


Engine::Engine()
{
	TCODConsole::initRoot(16, 16, "Roguelike");
	
	yPosition = 1;

	map = new int[16*16*16];

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			map[(i * 16 ^ 0) + (j * 16 ^ 1) + (0 * 256 ^ 2)] = 1;
		}
	}

	for (int i = 6; i < 12; i++) {
		for (int j = 6; j < 12; j++) {
			map[(i * 16 ^ 0) + (j * 16 ^ 1) + (1 * 256 ^ 2)] = 1;
		}
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
			int value = map[(i * 16 ^ 0) + (j * 16 ^ 1) + (1 * 256 ^ 2)];
			if (value == 0) {
				TCODConsole::root->setChar(i, j, '.');
			}
			else {
				TCODConsole::root->setChar(i, j, '#');
			}
		}
	}
}

void Engine::update()
{
	lastKey = TCODConsole::checkForKeypress(TCOD_KEY_PRESSED);
}