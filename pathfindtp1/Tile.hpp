#pragma once
#include "Position.hpp"

struct Tile
{
	Position position;
	bool walkable;

	int GetManhattanFromTarget(Tile* tile) const {
		return tile->position.CalculateManhattan(position);
	}
};