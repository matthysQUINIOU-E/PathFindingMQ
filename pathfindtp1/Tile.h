#pragma once
#include "Position.hpp"
#include "Node.hpp"

struct Tile
{
	Position position;
	bool walkable;

	int GetManhattanFromTarget(Tile* tile) const {
		return tile->position.CalculateManhattan(position);
	}
};

