#include "Map.h"



Map::Map(int w = 255, int h = 255, int d = 255) : width(w) , height(h) , depth(d)
{
	arr = new Tile[w*h*d];
}


Map::~Map()
{
	delete[] arr;
}




Map::Tile* Map::GetTileAt(Map::Pos p) {
	if (isTilePosValid(p))
		return &arr[(p.w - 1) + ((p.h - 1)*width) + ((p.d - 1)*height * width)];

	return nullptr;
}

Map::Tile* Map::GetTileAt(int w, int h, int d) {
	if (isTilePosValid(w,h,d)) 
		return &arr[(w - 1) + ((h - 1)*width) + ((d - 1)*height * width)];

	return nullptr;
}



bool Map::SetTileAt(const Pos p, Tile &tile) {
	if (isTilePosValid(p)) {
		arr[(p.w - 1) + ((p.h - 1)*width) + ((p.d - 1)*height * width)] = tile;
		return true;
	}
	return false;
}

bool Map::SetTileAt(const int w, const int h, const int d, Tile &tile) {
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