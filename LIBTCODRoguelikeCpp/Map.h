#pragma once

#include <libtcod.hpp>

class Map
{
public:

	class Pos {
	public:
		Pos(int w = 0, int h = 0, int d = 0) : w(w), h(h), d(d) {}

		Pos operator+(const Pos& p);
		Pos operator+(int v);
		Pos operator*(int f);

		int w;
		int h;
		int d;
	};

	class Tile {

	public: 

		enum Type {
			wall,
			empty,
			walkable
		} type = empty;

		TCODColor color = TCODColor::lightGrey;
		char c = ' ';
	};

	Map(int w, int h, int d);
	~Map();

	Tile* GetTileAt(Pos p);
	Tile* GetTileAt(int w, int h, int d);

	bool SetTileAt(const Pos p, Tile &tile);
	bool SetTileAt(const int w, const int h, const int d, Tile &tile);

	bool isTilePosValid(const Pos p);
	bool isTilePosValid(const int w, const int h, const int d);

	Tile * arr;

	int width, height, depth;

};

