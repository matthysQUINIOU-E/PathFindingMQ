#include "Algo.hpp"
#include <queue>

void DumbSearch(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::queue<Tile*> queue;
	Tile* fromTile = &tiles[from.y][from.x];
	queue.push(fromTile);

	Tile* toTile = &tiles[to.y][to.x];
	if (!toTile->walkable)
		return;

	while (!queue.empty() && toTile->cameFrom == nullptr)
	{
		Tile* tile = queue.front();
		queue.pop();
		std::vector<Position> neighbors = tile->position.GetNeighbors(minPos, maxPos);
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Position neighbor = neighbors[i];
			Tile* neighborTile = &tiles[neighbor.y][neighbor.x];

			if (neighbor == to) {
				neighborTile->cameFrom = tile;
				break;
			}

			if (!neighborTile->walkable)
				continue;

			if (neighborTile->visited)
				continue;

			neighborTile->visited = true;
			neighborTile->cameFrom = tile;
			queue.push(neighborTile);
		}
	}
	
	while (toTile->cameFrom != nullptr && toTile->position != from)
	{
		toTile->inPath = true;
		toTile = toTile->cameFrom;
	}
}

struct CompareDistanceFromStart
{
	bool operator()(Tile* a, Tile* b) {
		return a->distToStart > b->distToStart;
	}
};

void Bfs(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Tile*, std::vector<Tile*>, CompareDistanceFromStart> queue;

	Tile* fromTile = &tiles[from.y][from.x];
	queue.push(fromTile);
	fromTile->distToStart = 0;

	Tile* toTile = &tiles[to.y][to.x];
	if (!toTile->walkable)
		return;

	while (!queue.empty() && toTile->cameFrom == nullptr)
	{
		Tile* tile = queue.top();
		queue.pop();
		std::vector<Position> neighbors = tile->position.GetNeighbors(minPos, maxPos);
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Position neighbor = neighbors[i];
			Tile* neighborTile = &tiles[neighbor.y][neighbor.x];

			if (neighbor == to) {
				neighborTile->cameFrom = tile;
				break;
			}

			if (!neighborTile->walkable)
				continue;

			if (neighborTile->visited)
				continue;

			neighborTile->visited = true;
			neighborTile->cameFrom = tile;
			neighborTile->distToStart = tile->distToStart + 1;
			queue.push(neighborTile);
		}
	}

	while (toTile->cameFrom != nullptr && toTile->position != from)
	{
		toTile->inPath = true;
		toTile = toTile->cameFrom;
	}
}

struct CompareDistanceFromTarget
{
	bool operator()(Tile* a, Tile* b) {
		return a->distToTarget > b->distToTarget;
	}
};

void Dfs(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Tile*, std::vector<Tile*>, CompareDistanceFromTarget> queue;

	Tile* toTile = &tiles[to.y][to.x];
	if (!toTile->walkable)
		return;

	Tile* fromTile = &tiles[from.y][from.x];
	queue.push(fromTile);
	fromTile->CalculateManhattanFromTarget(toTile);

	while (!queue.empty() && toTile->cameFrom == nullptr)
	{
		Tile* tile = queue.top();
		queue.pop();
		std::vector<Position> neighbors = tile->position.GetNeighbors(minPos, maxPos);
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Position neighbor = neighbors[i];
			Tile* neighborTile = &tiles[neighbor.y][neighbor.x];

			if (neighbor == to) {
				neighborTile->cameFrom = tile;
				break;
			}

			if (!neighborTile->walkable)
				continue;

			if (neighborTile->visited)
				continue;

			neighborTile->visited = true;
			neighborTile->cameFrom = tile;
			neighborTile->CalculateManhattanFromTarget(toTile);
			queue.push(neighborTile);
		}
	}

	while (toTile->cameFrom != nullptr && toTile->position != from)
	{
		toTile->inPath = true;
		toTile = toTile->cameFrom;
	}
}

struct CompareAStar
{
	bool operator()(Tile* a, Tile* b) {
		return (a->distToTarget + a->distToStart) > (b->distToTarget + b->distToStart);
	}
};

void AStar(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Tile*, std::vector<Tile*>, CompareAStar> queue;

	Tile* toTile = &tiles[to.y][to.x];
	if (!toTile->walkable)
		return;

	Tile* fromTile = &tiles[from.y][from.x];
	queue.push(fromTile);
	fromTile->distToStart = 0;
	fromTile->CalculateManhattanFromTarget(toTile);

	while (!queue.empty() && toTile->cameFrom == nullptr)
	{
		Tile* tile = queue.top();
		queue.pop();
		std::vector<Position> neighbors = tile->position.GetNeighbors(minPos, maxPos);
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Position neighbor = neighbors[i];
			Tile* neighborTile = &tiles[neighbor.y][neighbor.x];

			if (neighbor == to) {
				neighborTile->cameFrom = tile;
				break;
			}

			if (!neighborTile->walkable)
				continue;

			if (neighborTile->visited)
				continue;

			neighborTile->visited = true;
			neighborTile->cameFrom = tile;
			neighborTile->distToStart = tile->distToStart + 1;
			neighborTile->CalculateManhattanFromTarget(toTile);
			queue.push(neighborTile);
		}
	}

	while (toTile->cameFrom != nullptr && toTile->position != from)
	{
		toTile->inPath = true;
		toTile = toTile->cameFrom;
	}
}