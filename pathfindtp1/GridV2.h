#pragma once
#include "Tile.h"
#include "Node.hpp"
#include "PathAlgo.hpp"
#include "Comparator.hpp"
#include <chrono>
#include <queue>
#include <unordered_map>

class GridV2
{
public:
	GridV2(int width, int height, PathAlgo pathAlgo);
	~GridV2();
	void Loop();

private:
	void PrintGrid();
	void HandleInput();

	void CalculatePath();
	void ClearPath();
	void SwitchPathAlgo();
	const char* GetPathAlgoName();

	void ResetMaze();
	void GenerateMaze();

	void BuildGraph();
	void DeleteNodes();

	template <typename T>
	void DumbSearch(std::vector<Node<T>*>& nodes, Node<T>* from, Node<T>* to)
	{
		Position minPos = { 0, 0 };

		std::queue<Node<T>*> queue;

		queue.push(from);

		while (!queue.empty() && to->cameFrom == nullptr)
		{
			Node<T>* node = queue.front();
			queue.pop();
			std::vector<Node<T>*> neighbors = node->neighbors;
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Node<T>* neighbor = neighbors[i];

				if (neighbor == to) {
					neighbor->cameFrom = node;
					break;
				}

				if (neighbor->visited)
					continue;

				neighbor->visited = true;
				neighbor->cameFrom = node;
				queue.push(neighbor);
			}
		}

		while (to->cameFrom != nullptr && to != from)
		{
			to->inPath = true;
			to = to->cameFrom;
		}
	}

	template <typename T>
	void Bfs(std::vector<Node<T>*>& nodes, Node<T>* from, Node<T>* to)
	{
		Position minPos = { 0, 0 };

		std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareBfs> queue;

		if (!to->walkable)
			return;

		from->distToStart = 0;
		queue.push(from);

		while (!queue.empty() && to->cameFrom == nullptr)
		{
			Node<T>* node = queue.top();
			queue.pop();

			std::vector<Node<T>*> neighbors = node->neighbors;
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Node<T>* neighbor = neighbors[i];

				if (neighbor == to) {
					neighbor->cameFrom = node;
					break;
				}

				if (neighbor->visited)
					continue;

				neighbor->visited = true;
				neighbor->cameFrom = node;
				neighbor->distToStart = node->distToStart + 1;
				queue.push(neighbor);
			}
		}

		while (to->cameFrom != nullptr && to != from)
		{
			to->inPath = true;
			to = to->cameFrom;
		}
	}

	template <typename T>
	void Dfs(std::vector<Node<T>*>& nodes, Node<T>* from, Node<T>* to)
	{
		Position minPos = { 0, 0 };

		std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareDfs> queue;

		if (!to->walkable)
			return;

		queue.push(from);

		from->CalculateDistToTarget(to);

		while (!queue.empty() && to->cameFrom == nullptr)
		{
			Node<T>* node = queue.top();
			queue.pop();

			std::vector<Node<T>*> neighbors = node->neighbors;
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Node<T>* neighbor = neighbors[i];

				if (neighbor == to) {
					neighbor->cameFrom = node;
					break;
				}

				if (neighbor->visited)
					continue;

				neighbor->visited = true;
				neighbor->cameFrom = node;
				neighbor->CalculateDistToTarget(to);
				queue.push(neighbor);
			}
		}

		while (to->cameFrom != nullptr && to != from)
		{
			to->inPath = true;
			to = to->cameFrom;
		}
	}

	template <typename T>
	void AStar(std::vector<Node<T>*>& nodes, Node<T>* from, Node<T>* to)
	{
		Position minPos = { 0, 0 };

		std::priority_queue<Node<T>*, std::vector<Node<T>*>, CompareAStar> queue;

		if (!to->walkable)
			return;

		queue.push(from);

		from->distToStart = 0;
		from->CalculateDistToTarget(to);

		while (!queue.empty() && to->cameFrom == nullptr)
		{
			Node<T>* node = queue.top();
			queue.pop();

			std::vector<Node<T>*> neighbors = node->neighbors;
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Node<T>* neighbor = neighbors[i];

				if (neighbor == to) {
					neighbor->cameFrom = node;
					break;
				}

				if (neighbor->visited)
					continue;

				neighbor->visited = true;
				neighbor->cameFrom = node;
				neighbor->distToStart = node->distToStart + 1;
				neighbor->CalculateDistToTarget(to);
				queue.push(neighbor);
			}
		}

		while (to->cameFrom != nullptr && to != from)
		{
			to->inPath = true;
			to = to->cameFrom;
		}
	}

	Node<Tile>* GetNodeCharacter();
	Node<Tile>* GetNodeCursor();

private:
	std::vector<std::vector<Tile>> m_tiles;
	std::vector<Node<Tile>*> m_graph;
	std::unordered_map<Tile*, Node<Tile>*> m_mapTilesGraph;

	Position m_cursorPos;
	Position m_characterPos;
	Position m_maxGridPos;

	PathAlgo m_pathAlgo;

	bool m_doPrint = true;

	std::chrono::microseconds m_algoExecutionTime;
};

