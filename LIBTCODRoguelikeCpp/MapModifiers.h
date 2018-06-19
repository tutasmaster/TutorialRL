#pragma once
#include "Map.h"

void SetMapLayer(Map& m, int l, Map::Tile tile);

void DrawSquareOnMap(Map& m, int x, int y, int w, int h, int d, Map::Tile tile);