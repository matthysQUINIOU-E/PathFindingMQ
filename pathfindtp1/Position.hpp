#pragma once
#include <iostream>
#include <vector>

struct Position
{
	int x;
	int y;

	void Print() const {
		std::cout << '(' << x << ',' << y << ')' << std::endl;
	}

	int CalculateManhattan(Position position) const {
		return abs(position.x - x) + abs(position.y - y);
	}

	bool operator== (Position& other) const {
		return x == other.x && y == other.y;
	}

	bool operator!= (Position& other) const {
		return x != other.x || y != other.y;
	}

	std::vector<Position> GetNeighbors(Position& min, Position& max) const {
		std::vector<Position> neighbors;
		if (x - 1 >= min.x)
			neighbors.push_back({ x - 1, y });
		if (x + 1 <= max.x)
			neighbors.push_back({ x + 1, y });
		if (y - 1 >= min.y)
			neighbors.push_back({ x, y - 1 });
		if (y + 1 <= max.y)
			neighbors.push_back({ x, y + 1 });
		return neighbors;
	}


};