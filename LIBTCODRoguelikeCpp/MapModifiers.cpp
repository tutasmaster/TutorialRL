#include "MapModifiers.h"

void SetMapLayer(Map& m, int l, Map::Tile tile){
	for (int j = 0; j < m.height; j++) {
		for (int i = 0; i < m.width; i++) {
			m.SetTileAt(i, j, l, tile);
		}
	}
}

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int d, Map::Tile tile) {
	for (int j = y; j < h + y; j++) {
		for (int i = x; i < w + x; i++) {
			m.SetTileAt(i, j, d, tile);
		}
	}
}