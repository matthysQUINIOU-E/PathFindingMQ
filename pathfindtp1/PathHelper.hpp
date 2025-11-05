/*
#pragma once
#include "Position.hpp"
#include <vector>

struct VisitedHelper
{
	Position at;
	std::vector<Position> moved;
	bool search;
};


struct PathHelper
{
	Position at;
	Position& to;
	int gridXSize;
	int gridYSize;
	std::vector<Position>& obstacles;
	std::vector<Position> moved;

	static std::vector<VisitedHelper> visiteds;

	static void ClearVisited() {
		visiteds.clear();
	}
	
	std::vector<Position> Search(size_t maxLenght = 100) {
		moved.push_back(at);
		int visitedIndex = GetVisitedIndex();
		if (visitedIndex == -2 || (at.x == to.x && at.y == to.y) || moved.size() > maxLenght) return moved;
		if (visitedIndex == -1)
			AddVisited();
		else
			moved = GetVisitedMoved(visitedIndex);

		std::vector<Position> neighbors = at.GetNeighbor();
		std::vector<Position> bestSearch;
		int bestSearchLenght = maxLenght;
		for (Position position : neighbors)
		{	
			if (position.x < 0 || position.y < 0 || position.x >= gridXSize || position.y >= gridYSize || IsInVectorPosition(obstacles, position))
				continue;
			std::vector<Position> search = PathHelper{ position, to, gridXSize, gridYSize, obstacles, moved}.Search(bestSearchLenght);
			if (search.size() < bestSearchLenght && !search.empty() && search.back().x == to.x && search.back().y == to.y) {
				bestSearchLenght = search.size();
				bestSearch = search;
			}
		}
		return bestSearch;
	}

	static void SetVisitedFlag() {
		for (int i = 0; i < PathHelper::visiteds.size(); i++)
		{
			PathHelper::visiteds[i].search = true;
		}
	}

private:
	int GetVisitedIndex() {
		for (int i = 0; i < PathHelper::visiteds.size(); i++)
		{
			VisitedHelper visited = PathHelper::visiteds[i];
			if (visited.at.x == at.x && visited.at.y == at.y && visited.moved.size() <= moved.size()) {
				if (!visited.search)
					return -2;
				PathHelper::visiteds[i].search = false;
				return i;
			}
		}
		return -1;
	}

	std::vector<Position> GetVisitedMoved(int index) {
		return PathHelper::visiteds[index].moved;
	}

	void AddVisited() {
		for (int i = 0; i < PathHelper::visiteds.size(); i++)
		{
			VisitedHelper visited = PathHelper::visiteds[i];
			if (visited.at.x == at.x && visited.at.y == at.y && visited.moved.size() > moved.size()) {
				VisitedHelper visitedHelper = { at, moved };
				PathHelper::visiteds[i] = visitedHelper;
			}
		}
		VisitedHelper visitedHelper = { at, moved };
		PathHelper::visiteds.push_back(visitedHelper);
	}



	bool IsInVectorPosition(std::vector<Position> vector, Position position) {
		for (size_t i = 0; i < vector.size(); i++)
		{
			Position pathPosition = vector[i];
			if (pathPosition.x == position.x && pathPosition.y == position.y)
				return true;
		}
		return false;
	}
};

*/