#include "GridV2.h"
#include "Ascii.hpp"
#include "Comparator.hpp"
#include <Windows.h>
#include <conio.h>
#include <random>
#include <string>
#include <queue>
#include <unordered_map>

GridV2::GridV2(int width, int height, PathAlgo pathAlgo) {
	m_pathAlgo = pathAlgo;

	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	m_maxGridPos = { width - 1, height - 1 };

	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> distributionX(0, m_maxGridPos.x);
	std::uniform_int_distribution<int> distributionY(0, m_maxGridPos.y);

	Position startPos = { distributionX(mt), distributionY(mt) };

	m_characterPos = startPos;
	m_cursorPos = startPos;

	for (int y = 0; y < height; y++)
	{
		m_tiles.push_back(std::vector<Tile>());
		for (int x = 0; x < width; x++)
		{
			Tile newTile = { {x,y}, false, false, false };
			m_tiles[y].push_back(newTile);
		}
	}

	GenerateMaze();
	BuildGraph();
}

GridV2::~GridV2() {
	DeleteNodes();
}

void GridV2::Loop() {
	while (true)
	{
		if (m_doPrint)
			PrintGrid();
		HandleInput();
	}
}

void GridV2::PrintGrid() {
	system("cls");
	std::string buffer;

	buffer += "\x1b[47m"; // bg blanc
	for (int x = 0; x <= m_maxGridPos.x + 2; x++)
	{
		buffer += ' ';
	}
	buffer += "\x1b[0m\n"; // reset

	for (int y = 0; y <= m_maxGridPos.y; y++) {
		buffer += "\x1b[47m "; // bg blanc
		for (int x = 0; x <= m_maxGridPos.x; x++) {

			Tile& tile = m_tiles[y][x];

			if (x == m_characterPos.x && y == m_characterPos.y)
				buffer += "\x1b[44m "; //bg bleu
			else if (x == m_cursorPos.x && y == m_cursorPos.y)
				buffer += "\x1b[46m "; //bg cyan
			else if (tile.inPath)
				buffer += "\x1b[42m "; //bg vert
			else if (!tile.walkable)
				buffer += "\x1b[41m "; //bg rouge
			else
				buffer += "\x1b[0m "; // reset
		}
		buffer += "\x1b[47m "; // bg blanc
		buffer += "\x1b[0m\n"; // reset
	}

	buffer += "\x1b[47m"; // bg blanc
	for (int x = 0; x <= m_maxGridPos.x + 2; x++)
	{
		buffer += ' ';
	}
	buffer += "\x1b[0m\n"; // reset

	buffer += "R - Reload maze, ZQSD or WASD - move, ENTER - go to cursor, TAB - switch Path Algorithm, F - Calculate Path Again\n";
	buffer += "Current Algorithm : ";
	buffer += GetPathAlgoName();
	buffer += ", execution time : " + std::to_string(m_algoExecutionTime.count());
	buffer += " micro s";

	std::cout << buffer;

	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		node->data->position.Print();
	}

	m_doPrint = false;
}

void GridV2::HandleInput() {
	auto input = _getch();
	switch (input)
	{
	case Ascii::TAB:
		SwitchPathAlgo();
		break;
	case Ascii::ENTER:
		if (!m_tiles[m_cursorPos.y][m_cursorPos.x].inPath)
			break;
		ClearPath();
		m_characterPos = m_cursorPos;
		break;
	case Ascii::R:
		ResetMaze();
		break;
	case Ascii::F:
		CalculatePath();
		break;
	case Ascii::W:
	case Ascii::Z:
		m_cursorPos.y--;
		if (m_cursorPos.y < 0)
			m_cursorPos.y = m_maxGridPos.y;
		CalculatePath();
		break;
	case Ascii::A:
	case Ascii::Q:
		m_cursorPos.x--;
		if (m_cursorPos.x < 0)
			m_cursorPos.x = m_maxGridPos.x;
		CalculatePath();
		break;
	case Ascii::S:
		m_cursorPos.y++;
		m_cursorPos.y %= m_maxGridPos.y + 1;
		CalculatePath();
		break;
	case Ascii::D:
		m_cursorPos.x++;
		m_cursorPos.x %= m_maxGridPos.x + 1;
		CalculatePath();
		break;
	default:
		break;
	}
}



void GridV2::CalculatePath() {
	ClearPath();
	std::chrono::steady_clock::time_point start; 
	std::chrono::steady_clock::time_point end;
	switch (m_pathAlgo)
	{
	case PathAlgo::DUMB_SEARCH:
		start = std::chrono::high_resolution_clock::now();
		DumbSearch(m_graph, m_characterPos, m_cursorPos, m_maxGridPos);
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::BFS:
		start = std::chrono::high_resolution_clock::now();
		Bfs(m_graph, m_characterPos, m_cursorPos, m_maxGridPos);
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::DFS:
		start = std::chrono::high_resolution_clock::now();
		Dfs(m_graph, m_characterPos, m_cursorPos, m_maxGridPos);
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::A_STAR:
		start = std::chrono::high_resolution_clock::now();
		AStar(m_graph, m_characterPos, m_cursorPos, m_maxGridPos);
		end = std::chrono::high_resolution_clock::now();
		break;
	default:
		start = std::chrono::high_resolution_clock::now();
		end = std::chrono::high_resolution_clock::now();
		break;
	}

	m_algoExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

void GridV2::ClearPath() {
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		node->visited = false;
		node->inPath = false;
		node->cameFrom = nullptr;
	}

	m_doPrint = true;
}

void GridV2::SwitchPathAlgo() {
	m_pathAlgo = (PathAlgo)((m_pathAlgo + 1) % PathAlgo::SIZE_PATH_ALGO);
	CalculatePath();
}

const char* GridV2::GetPathAlgoName() {
	switch (m_pathAlgo) {
	case PathAlgo::DUMB_SEARCH: return "Simple";
	case PathAlgo::A_STAR: return "AStar";
	case PathAlgo::DFS: return "DFS";
	case PathAlgo::BFS: return "BFS";
	default: return "Unknown";
	}
}

void GridV2::ResetMaze() {
	for (size_t y = 0; y <= m_maxGridPos.y; y++)
	{
		for (size_t x = 0; x <= m_maxGridPos.x; x++)
		{
			Tile& tile = m_tiles[y][x];
			tile.walkable = false;
		}
	}

	GenerateMaze();
	CalculatePath();
}

void GridV2::GenerateMaze() {
	Position minPos = { 0, 0 };

	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector<Tile*> frontiers;
	Tile* tile = &m_tiles[m_characterPos.y][m_characterPos.x];
	tile->walkable = true;

	std::vector<Position> neighbors = tile->position.GetNeighbors(minPos, m_maxGridPos);
	for (size_t i = 0; i < neighbors.size(); i++)
	{
		Position neighbor = neighbors[i];
		frontiers.push_back(&m_tiles[neighbor.y][neighbor.x]);
	}

	while (!frontiers.empty())
	{
		std::uniform_int_distribution<int> distribution(0, static_cast<int>(frontiers.size() - 1));
		int randomIndex = distribution(mt);
		Tile* tile = frontiers[randomIndex];

		std::swap(frontiers[randomIndex], frontiers.back());
		frontiers.pop_back();

		int passageCount = 0;
		std::vector<Position> neighbors = tile->position.GetNeighbors(minPos,m_maxGridPos);
		std::vector<Tile*> walledNeighbors;

		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Position neighbor = neighbors[i];
			Tile* neighborTile = &m_tiles[neighbor.y][neighbor.x];
			if (!neighborTile->walkable)
				walledNeighbors.push_back(neighborTile);
		}

		if (neighbors.size() - walledNeighbors.size() == 1) {
			tile->walkable = true;
			for (size_t i = 0; i < walledNeighbors.size(); i++)
			{
				Tile* neighborTile = walledNeighbors[i];
				frontiers.push_back(neighborTile);
			}
		}

	}
}


void GridV2::BuildGraph() {
	Position minGridPos = { 0, 0 };

	std::unordered_map<Tile*, Node<Tile>*> tilesNodesMap;

	for (size_t y = 0; y <= m_maxGridPos.y; y++)
	{
		for (size_t x = 0; x <= m_maxGridPos.x; x++)
		{
			Tile* tile = &m_tiles[y][x];
			Node<Tile>* node = nullptr;

			auto it = tilesNodesMap.find(tile);
			if (it != tilesNodesMap.end())
				node = it->second;
			else {
				node = new Node<Tile> { tile };
				tilesNodesMap[tile] = node;
			}

			std::vector<Position> neighbors = tile->position.GetNeighbors(minGridPos, m_maxGridPos);
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Position neighbor = neighbors[i];
				Tile* neighborTile = &m_tiles[neighbor.y][neighbor.x];
				
				if (neighborTile->walkable)
				{
					Node<Tile>* neighborNode = nullptr;
					auto neighborIt = tilesNodesMap.find(neighborTile);
					if (neighborIt != tilesNodesMap.end())
						neighborNode = neighborIt->second;
					else {
						neighborNode = new Node<Tile>{ tile };
						tilesNodesMap[neighborTile] = neighborNode;
					}

					node->neighbors.push_back(neighborNode);
				}
			}
			m_graph.push_back(node);
		}
	}
}

void GridV2::DeleteNodes() {
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		delete node;
	}
	m_graph.clear();
}

void GridV2::DumbSearch(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::queue<Node<Tile>*> queue;

	Node<Tile>* toNode = GetNodeCursor();
	if (!toNode->data->walkable)
		return;

	Node<Tile>* fromNode = GetNodeCharacter();
	queue.push(fromNode);

	while (!queue.empty() && toNode->cameFrom == nullptr)
	{
		Node<Tile>* node = queue.front();
		queue.pop();
		std::vector<Node<Tile>*> neighbors = node->neighbors;
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node<Tile>* neighbor = neighbors[i];

			if (neighbor == toNode) {
				neighbor->cameFrom = node;
				break;
			}

			if (!neighbor->data->walkable)
				continue;

			if (neighbor->visited)
				continue;

			neighbor->visited = true;
			neighbor->cameFrom = node;
			queue.push(neighbor);
		}
	}

	while (toNode->cameFrom != nullptr && toNode != fromNode)
	{
		toNode->inPath = true;
		toNode = toNode->cameFrom;
	}
}



void GridV2::Bfs(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Node<Tile>*, std::vector<Node<Tile>*>, CompareBfs> queue;

	Node<Tile>* toNode = GetNodeCursor();
	if (!toNode->data->walkable)
		return;


	Node<Tile>* fromNode = GetNodeCharacter();
	fromNode->data->distToStart = 0;
	queue.push(fromNode);

	while (!queue.empty() && toNode->cameFrom == nullptr)
	{
		Node<Tile>* node = queue.top();
		queue.pop();

		std::vector<Node<Tile>*> neighbors = node->neighbors;
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node<Tile>* neighbor = neighbors[i];

			if (neighbor == toNode) {
				neighbor->cameFrom = node;
				break;
			}

			if (!neighbor->data->walkable)
				continue;

			if (neighbor->visited)
				continue;

			neighbor->visited = true;
			neighbor->cameFrom = node;
			neighbor->data->distToStart = node->data->distToStart + 1;
			queue.push(neighbor);
		}
	}

	while (toNode->cameFrom != nullptr && toNode != fromNode)
	{
		toNode->inPath = true;
		toNode = toNode->cameFrom;
	}
}

void GridV2::Dfs(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Node<Tile>*, std::vector<Node<Tile>*>, CompareDfs> queue;


	Node<Tile>* toNode = GetNodeCursor();
	if (!toNode->data->walkable)
		return;


	Node<Tile>* fromNode = GetNodeCharacter();
	queue.push(fromNode);

	fromNode->data->CalculateManhattanFromTarget(toNode->data);

	while (!queue.empty() && toNode->cameFrom == nullptr)
	{
		Node<Tile>* node = queue.top();
		queue.pop();

		std::vector<Node<Tile>*> neighbors = node->neighbors;
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node<Tile>* neighbor = neighbors[i];

			if (neighbor == toNode) {
				neighbor->cameFrom = node;
				break;
			}

			if (!neighbor->data->walkable)
				continue;

			if (neighbor->visited)
				continue;

			neighbor->visited = true;
			neighbor->cameFrom = node;
			neighbor->data->CalculateManhattanFromTarget(toNode->data);
			queue.push(neighbor);
		}
	}

	while (toNode->cameFrom != nullptr && toNode != fromNode)
	{
		toNode->inPath = true;
		toNode = toNode->cameFrom;
	}
}

void GridV2::AStar(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos) {
	Position minPos = { 0, 0 };

	std::priority_queue<Node<Tile>*, std::vector<Node<Tile>*>, CompareAStar> queue;

	Node<Tile>* fromNode = GetNodeCursor();
	queue.push(fromNode);

	Node<Tile>* toNode = GetNodeCharacter();
	if (!toNode->data->walkable)
		return;

	fromNode->data->distToStart = 0;
	fromNode->data->CalculateManhattanFromTarget(toNode->data);

	while (!queue.empty() && toNode->cameFrom == nullptr)
	{
		Node<Tile>* node = queue.top();
		queue.pop();

		std::vector<Node<Tile>*> neighbors = node->neighbors;
		for (size_t i = 0; i < neighbors.size(); i++)
		{
			Node<Tile>* neighbor = neighbors[i];

			if (neighbor == toNode) {
				neighbor->cameFrom = node;
				break;
			}

			if (!neighbor->data->walkable)
				continue;

			if (neighbor->visited)
				continue;

			neighbor->visited = true;
			neighbor->cameFrom = node;
			neighbor->data->distToStart = node->data->distToStart + 1;
			neighbor->data->CalculateManhattanFromTarget(toNode->data);
			queue.push(neighbor);
		}
	}

	while (toNode->cameFrom != nullptr && toNode != fromNode)
	{
		toNode->inPath = true;
		toNode = toNode->cameFrom;
	}
}

Node<Tile>* GridV2::GetNodeCharacter() {
	Tile* tileCharacter = &m_tiles[m_characterPos.y][m_characterPos.x];
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		if (node->data->position == tileCharacter->position)
			return node;
	}
	return nullptr;
}

Node<Tile>* GridV2::GetNodeCursor() {
	Tile* tileCursor = &m_tiles[m_cursorPos.y][m_cursorPos.x];
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		if (node->data->position == tileCursor->position)
			return node;
	}
	return nullptr;
}