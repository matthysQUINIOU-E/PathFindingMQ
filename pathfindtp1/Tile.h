#pragma once
#include "Position.hpp"

struct Tile
{
	Position position;
	bool walkable;
	bool visited;
	bool inPath;
	int distToStart;
	int distToTarget;
	Tile* cameFrom;

	void CalculateManhattanFromTarget(Tile* startTile) {
		distToTarget = position.CalculateManhattan(startTile->position);
	}
};

