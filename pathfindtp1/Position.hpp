#pragma once
#include <iostream>
#include <vector>

struct Position
{
	int x;
	int y;

	void Print() {
		std::cout << '(' << x << ',' << y << ')' << std::endl;
	}

	int CalculateManhattan(Position position) {
		return abs(position.x - x) + abs(position.y - y);
	}

	std::vector<Position> GetNeighbor() {
		return {
			{x - 1, y},
			{x + 1, y},
			{x, y - 1},
			{x, y + 1}
		};
	}
};