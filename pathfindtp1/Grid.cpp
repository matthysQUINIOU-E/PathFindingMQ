#include "Grid.h"
#include "Ascii.hpp"
#include "Comparator.hpp"
#include "Algo.hpp"
#include <Windows.h>
#include <conio.h>
#include <random>
#include <string>
#include <queue>
#include <unordered_map>

Grid::Grid(int width, int height, PathAlgo pathAlgo) {
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
			Tile newTile = { {x,y}, false };
			m_tiles[y].push_back(newTile);
		}
	}

	GenerateMaze();
	BuildGraph();
}

Grid::~Grid() {
	DeleteNodes();
}

void Grid::Loop() {
	while (true)
	{
		if (m_doPrint)
			PrintGrid();
		HandleInput();
	}
}

void Grid::PrintGrid() {
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
			else if (m_mapTilesGraph[&tile]->inPath)
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

	buffer += "R - Reload maze, ZQSD/WASD/Arrows - move, ENTER - go to cursor, TAB - switch Path Algorithm, F - Calculate Path Again\n";
	buffer += "Current Algorithm : ";
	buffer += Algo::GetPathAlgoName(m_pathAlgo);
	buffer += ", execution time : " + std::to_string(m_algoExecutionTime.count());
	buffer += " micro s";

	std::cout << buffer;

	m_doPrint = false;
}

void Grid::HandleInput() {
	auto input = _getch();
	switch (input)
	{
	case Ascii::TAB:
		SwitchPathAlgo();
		break;
	case Ascii::ENTER: {
		Tile* cursorTile = &m_tiles[m_cursorPos.y][m_cursorPos.x];
		if (!m_mapTilesGraph[cursorTile]->inPath)
			break;
		ClearPath();
		m_characterPos = m_cursorPos;
		break;
	}
	case Ascii::R:
		ResetMaze();
		break;
	case Ascii::F:
		CalculatePath();
		break;
	case Ascii::W:
	case Ascii::Z:
	case Ascii::UP:
		m_cursorPos.y--;
		if (m_cursorPos.y < 0)
			m_cursorPos.y = m_maxGridPos.y;
		CalculatePath();
		break;
	case Ascii::A:
	case Ascii::Q:
	case Ascii::LEFT:
		m_cursorPos.x--;
		if (m_cursorPos.x < 0)
			m_cursorPos.x = m_maxGridPos.x;
		CalculatePath();
		break;
	case Ascii::S:
	case Ascii::DOWN:
		m_cursorPos.y++;
		m_cursorPos.y %= m_maxGridPos.y + 1;
		CalculatePath();
		break;
	case Ascii::D:
	case Ascii::RIGHT:
		m_cursorPos.x++;
		m_cursorPos.x %= m_maxGridPos.x + 1;
		CalculatePath();
		break;
	default:
		std::cout << input << std::endl;
		break;
	}
}



void Grid::CalculatePath() {
	ClearPath();
	std::chrono::steady_clock::time_point start; 
	std::chrono::steady_clock::time_point end;
	switch (m_pathAlgo)
	{
	case PathAlgo::DUMB_SEARCH:
		start = std::chrono::high_resolution_clock::now();
		Algo::DumbSearch(m_graph, GetNodeCharacter(), GetNodeCursor());
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::BFS:
		start = std::chrono::high_resolution_clock::now();
		Algo::Bfs(m_graph, GetNodeCharacter(), GetNodeCursor());
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::DFS:
		start = std::chrono::high_resolution_clock::now();
		Algo::Dfs(m_graph, GetNodeCharacter(), GetNodeCursor());
		end = std::chrono::high_resolution_clock::now();
		break;
	case PathAlgo::A_STAR:
		start = std::chrono::high_resolution_clock::now();
		Algo::AStar(m_graph, GetNodeCharacter(), GetNodeCursor());
		end = std::chrono::high_resolution_clock::now();
		break;
	default:
		start = std::chrono::high_resolution_clock::now();
		end = std::chrono::high_resolution_clock::now();
		break;
	}

	m_algoExecutionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
}

void Grid::ClearPath() {
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		node->visited = false;
		node->inPath = false;
		node->cameFrom = nullptr;
	}

	m_doPrint = true;
}

void Grid::SwitchPathAlgo() {
	m_pathAlgo = (PathAlgo)((m_pathAlgo + 1) % PathAlgo::SIZE_PATH_ALGO);
	CalculatePath();
}



void Grid::ResetMaze() {
	for (size_t y = 0; y <= m_maxGridPos.y; y++)
	{
		for (size_t x = 0; x <= m_maxGridPos.x; x++)
		{
			Tile& tile = m_tiles[y][x];
			tile.walkable = false;
		}
	}

	GenerateMaze();
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		node->neighbors.clear();
	}
	BuildGraph();
	CalculatePath();
}

void Grid::GenerateMaze() {
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


void Grid::BuildGraph() {
	Position minGridPos = { 0, 0 };

	for (size_t y = 0; y <= m_maxGridPos.y; y++)
	{
		for (size_t x = 0; x <= m_maxGridPos.x; x++)
		{
			Tile* tile = &m_tiles[y][x];
			Node<Tile>* node = nullptr;
			auto it = m_mapTilesGraph.find(tile);
			if (it == m_mapTilesGraph.end()) {
				node = new Node<Tile>{ tile, tile->walkable };
				m_mapTilesGraph[tile] = node;
			}
			else {
				node = it->second;
			};
			node->walkable = node->data->walkable;

			std::vector<Position> neighbors = tile->position.GetNeighbors(minGridPos, m_maxGridPos);
			for (size_t i = 0; i < neighbors.size(); i++)
			{
				Position neighbor = neighbors[i];
				Tile* neighborTile = &m_tiles[neighbor.y][neighbor.x];
				
				if (neighborTile->walkable)
				{
					Node<Tile>* neighborNode = nullptr;
					auto it = m_mapTilesGraph.find(neighborTile);
					if (it == m_mapTilesGraph.end()) {
						neighborNode = new Node<Tile>{ neighborTile, neighborTile->walkable };
						m_mapTilesGraph[neighborTile] = neighborNode;
					}
					else {
						neighborNode = it->second;
					};

					node->neighbors.push_back(neighborNode);
				}
			}
			m_graph.push_back(node);
		}
	}
}

void Grid::DeleteNodes() {
	for (size_t i = 0; i < m_graph.size(); i++)
	{
		Node<Tile>* node = m_graph[i];
		delete node;
	}
	m_graph.clear();
}

Node<Tile>* Grid::GetNodeCharacter() {
	Tile* tileCharacter = &m_tiles[m_characterPos.y][m_characterPos.x];
	return m_mapTilesGraph[tileCharacter];
}

Node<Tile>* Grid::GetNodeCursor() {
	Tile* tileCursor = &m_tiles[m_cursorPos.y][m_cursorPos.x];
	return m_mapTilesGraph[tileCursor];
}