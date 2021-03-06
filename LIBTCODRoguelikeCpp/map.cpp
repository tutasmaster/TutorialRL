#include "map.hpp"

#include "engine.hpp"

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int MAX_ROOM_MONSTERS = 3;
static const int MAX_ROOM_ITEMS = 2;

class BspListener : public ITCODBspCallback {
private:
	Map & map; // a map to dig
	int roomNum; // room number
	int lastx, lasty; // center of the last room
public:
	BspListener(Map &map) : map(map), roomNum(0) {}
	bool visitNode(TCODBsp *node, void *userData) {
		if (node->isLeaf()) {
			int x, y, w, h;
			// dig a room
			bool withActors = (bool)userData;
			w = map.rng->getInt(ROOM_MIN_SIZE, node->w - 2);
			h = map.rng->getInt(ROOM_MIN_SIZE, node->h - 2);
			x = map.rng->getInt(node->x + 1, node->x + node->w - w - 1);
			y = map.rng->getInt(node->y + 1, node->y + node->h - h - 1);
			map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1, withActors);
			if (roomNum != 0) {
				// dig a corridor from last room
				map.dig(lastx, lasty, x + w / 2, lasty);
				map.dig(x + w / 2, lasty, x + w / 2, y + h / 2);
			}
			lastx = x + w / 2;
			lasty = y + h / 2;
			roomNum++;
		}
		return true;
	}
};

Map::Map(int width, int height)
	: width(width), height(height) {
	seed = TCODRandom::getInstance()->getInt(0, 0x7FFFFFFF);
}

void Map::init(bool withActors) {
	rng = new TCODRandom(seed, TCOD_RNG_CMWC);
	tiles = new Tile[width*height];
	map = new TCODMap(width, height);
	TCODBsp bsp(0, 0, width, height);
	bsp.splitRecursive(rng, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
	BspListener listener(*this);
	bsp.traverseInvertedLevelOrder(&listener, (void *)withActors);
}

Map::~Map() {
	delete [] tiles;
	map->clear();
}

void Map::dig(int x1, int y1, int x2, int y2) {
	if (x2 < x1) {
		int tmp = x2;
		x2 = x1;
		x1 = tmp;
	}
	if (y2 < y1) {
		int tmp = y2;
		y2 = y1;
		y1 = tmp;
	}
	for (int tilex = x1; tilex <= x2; tilex++) {
		for (int tiley = y1; tiley <= y2; tiley++) {
			map->setProperties(tilex, tiley, true, true);
		}
	}
}

void Map::addMonster(int x, int y) {
	TCODRandom *rng = TCODRandom::getInstance();
	if (rng->getInt(0, 100) < 80) {
		// create an orc
		Actor *orc = new Actor(x, y,true, 'o', "Orc",
			TCODColor::darkRed);
		orc->destructible = new Destructible(10, 0,35, "Dead Orc");
		orc->attacker = new Attacker(30);
		orc->ai = new EnemyAi();
		engine.actors.push(orc);
	}
	else {
		// create a troll
		Actor *troll = new Actor(x, y, true, 'T', "Troll",
			TCODColor::darkRed);
		troll->destructible = new Destructible(60, 1, 100, "Troll Carcass" );
		troll->attacker = new Attacker(50);
		troll->ai = new EnemyAi();
		engine.actors.push(troll);
	}
}

void Map::addItem(int x, int y) {
	TCODRandom *rng = TCODRandom::getInstance();
	int dice = rng->getInt(0, 100);
	if (dice < 60) {
		// create a health potion
		Actor *healthPotion = new Actor(x, y,false, '!', "Potion",TCODColor::green);
		healthPotion->solid = false;
		healthPotion->pickable = new HealthPickable();
		engine.actors.insertBefore(healthPotion,0);
	}
	else if (dice < 60 + 20) {
		// create a bow
		Actor *bow = new Actor(x, y,false, ')', "Bow",
			TCODColor::lightYellow);
		bow->pickable = new BowPickable ();
		bow->attacker = new Attacker(60);
		engine.actors.push(bow);
	}
	else if (dice < 60 + 40) {
		// create a scroll of fireball
		Actor *arrow = new Actor(x, y, false, '-', "Arrow",
			TCODColor::white);
		arrow->ai = new ArrowAi();
		arrow->attacker = new Attacker(60);
		arrow->pickable = new Pickable();
		engine.actors.push(arrow);
	}/*
	else {
		// create a scroll of confusion
		Actor *scrollOfConfusion = new Actor(x, y, '#', "scroll of confusion",
			TCODColor::lightYellow);
		scrollOfConfusion->solid = false;
		scrollOfConfusion->pickable = new Confuser(10, 8);
		engine.actors.push(scrollOfConfusion);
	}*/
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2, bool withActors) {
	dig(x1, y1, x2, y2);
	if (!withActors) {
		return;
	}
	if (first) {
		// put the player in the first room
		engine.player->x = (x1 + x2) / 2;
		engine.player->y = (y1 + y2) / 2;
	}
	else {
		TCODRandom *rng = TCODRandom::getInstance();
		// add monsters
		int nbMonsters = rng->getInt(0, MAX_ROOM_MONSTERS);
		while (nbMonsters > 0) {
			int x = rng->getInt(x1, x2);
			int y = rng->getInt(y1, y2);
			if (canWalk(x, y)) {
				addMonster(x, y);
			}
			nbMonsters--;
		}
		// add items
		int nbItems = rng->getInt(0, MAX_ROOM_ITEMS);
		while (nbItems > 0) {
			int x = rng->getInt(x1, x2);
			int y = rng->getInt(y1, y2);
			if (canWalk(x, y)) {
				addItem(x, y);
			}
			nbItems--;
		}
		// set stairs position
		engine.stairs->x = (x1 + x2) / 2;
		engine.stairs->y = (y1 + y2) / 2;
	}
}

bool Map::isWall(int x, int y) const {
	return !map->isWalkable(x, y);
}

bool Map::canWalk(int x, int y) const {
	if (isWall(x, y)) {
		// this is a wall
		return false;
	}
	for (Actor **iterator = engine.actors.begin();
		iterator != engine.actors.end(); iterator++) {
		Actor *actor = *iterator;
		if (actor->solid && actor->x == x && actor->y == y) {
			// there is a blocking actor here. cannot walk
			return false;
		}
	}
	return true;
}

bool Map::isExplored(int x, int y) const {
	return tiles[x + y * width].explored;
}

bool Map::isInFov(int x, int y) const {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return false;
	}
	if (map->isInFov(x, y)) {
		tiles[x + y * width].explored = true;
		return true;
	}
	return false;
}

void Map::computeFov() {
	map->computeFov(engine.player->x, engine.player->y,
		engine.fovRadius);
}

void Map::render() const {
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {

			int distance = sqrt(((engine.player->x - x) * (engine.player->x - x)) + ((engine.player->y - y) * (engine.player->y - y)));
			TCODColor lightGround(200, 180, 50);
			lightGround.setHue(60 - (distance*4));
			//lightGround.setSaturation(255 - distance);
			TCODColor lightWall(130, 110, 50);
			lightWall.setHue(60 - (distance * 4));
			TCODColor darkGround(50, 50, 150);
			darkGround.setHue(200 + (distance/2));
			TCODColor darkWall(0, 0, 100);
			
			TCODColor blankWall(0, 0, 0);
			blankWall.setValue((distance/2)+200);
			if (isInFov(x, y)) {
				engine.con.setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
			}
			else if (isExplored(x, y)) {
				engine.con.setCharBackground(x, y,
					isWall(x, y) ? darkWall : darkGround);
			}
			else {
				engine.con.setCharBackground(x, y, blankWall);
			}
		}
	}
}
