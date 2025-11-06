#pragma once
#include "Position.hpp"

struct Tile
{
	Position position;
	bool walkable;
	bool inPath;
	int distToStart;
	int distToTarget;

	void CalculateManhattanFromTarget(Tile* startTile) {
		distToTarget = position.CalculateManhattan(startTile->position);
	}
};

