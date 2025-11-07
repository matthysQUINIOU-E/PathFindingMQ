#pragma once
#include "Node.hpp"
#include "Comparator.hpp"
#include <queue>

namespace Algo {
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

	const char* GetPathAlgoName(PathAlgo pathAlgo) {
		switch (pathAlgo) {
		case PathAlgo::DUMB_SEARCH: return "Simple";
		case PathAlgo::A_STAR: return "AStar";
		case PathAlgo::DFS: return "DFS";
		case PathAlgo::BFS: return "BFS";
		default: return "Unknown";
		}
	}
}