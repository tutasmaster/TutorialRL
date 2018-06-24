#include "Map.h"


Map::Map(int w = 255, int h = 255, int d = 255) : width(w) , height(h) , depth(d)
{
	arr = new TileID[w*h*d];

	for (int i = 0; i < w*h*d; i++) {
		arr[i] = 0;
	}
}


Map::~Map()
{
	delete[] arr;
}

Tile* Map::GetTileAt(Pos pos) {
	if ((pos.w > -1 && pos.h > -1 && pos.d > -1) &&
		(pos.w < width && pos.h < height && pos.d < depth))
		return tileManager.GetTileData(arr[pos.w + (pos.h * width) + (pos.d * width * height)]);
	return nullptr;
}

Tile* Map::GetTileAt(int w, int h, int d) {
	if ((w > -1 && h > -1 && d > -1) &&
		(w < width && h < height && d < depth))
		return tileManager.GetTileData(arr[w + (h * width) + (d * width * height)]);
	return nullptr;
}


bool Map::SetTileAt(const Pos p, TileID tile) {
	if (isTilePosValid(p)) {
		arr[(p.w - 1) + ((p.h - 1)*width) + ((p.d - 1)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetTileAt(const int w, const int h, const int d, TileID tile) {
	if(isTilePosValid(w,h,d)){
		arr[(w - 1) + ((h - 1)*width) + ((d - 1)*height * width)] = tile;
		return true;
	}
	return false;
}



bool Map::isTilePosValid(Pos p) {
	return ((p.w > 0 && p.h > 0 && p.d > 0) && (p.w < width + 1 && p.h < height + 1 && p.d < depth + 1));
}

bool Map::isTilePosValid(int w, int h, int d) {
	return ((w > 0 && h > 0 && d > 0) && (w < width + 1 && h < height + 1 && d < depth + 1));
}



Map::Pos Map::Pos::operator+(const Pos& p) {
	Pos v(0,0,0);

	v.w = p.w + this->w;
	v.h = p.h + this->h;
	v.d = p.d + this->d;

	return v;
}

Map::Pos Map::Pos::operator+(int v) {
	Pos n(0, 0, 0);

	n.w = v + this->w;
	n.h = v + this->h;
	n.d = v + this->d;

	return n;
}

Map::Pos Map::Pos::operator*(int f) {
	Pos n(0, 0, 0);

	n.w = f * this->w;
	n.h = f * this->h;
	n.d = f * this->d;

	return n;
}

void SetMapLayer(Map& m, int l, TileID tile) {
	for (int j = 0; j < m.height; j++) {
		for (int i = 0; i < m.width; i++) {
			m.SetTileAt(i, j, l, tile);
		}
	}
}

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int d, TileID tile) {
	for (int j = y; j < h + y; j++) {
		for (int i = x; i < w + x; i++) {
			m.SetTileAt(i, j, d, tile);
		}
	}
}