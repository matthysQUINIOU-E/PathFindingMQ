#include "GridV2.h"
#include "Ascii.hpp"
#include "Algo.hpp"
#include <Windows.h>
#include <conio.h>
#include <random>

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

	buffer += "R - Reload maze, ZQSD or WASD move, ENTER go to cursor";

	std::cout << buffer;

	m_doPrint = false;
}

void GridV2::HandleInput() {
	auto input = _getch();
	switch (input)
	{
	case Ascii::ENTER:
		if (!m_tiles[m_cursorPos.y][m_cursorPos.x].inPath)
			break;
		ClearPath();
		m_characterPos = m_cursorPos;
		break;
	case Ascii::R:
		ResetMaze();
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
	switch (m_pathAlgo)
	{
	case PathAlgo::DUMB_SEARCH:
		DumbSearch(m_tiles, m_characterPos, m_cursorPos, m_maxGridPos);
		break;
	case PathAlgo::BFS:
		Bfs(m_tiles, m_characterPos, m_cursorPos, m_maxGridPos);
		break;
	case PathAlgo::DFS:
		Dfs(m_tiles, m_characterPos, m_cursorPos, m_maxGridPos);
		break;
	case PathAlgo::A_STAR:
		AStar(m_tiles, m_characterPos, m_cursorPos, m_maxGridPos);
		break;
	default:
		break;
	}
}

void GridV2::ClearPath() {
	for (size_t y = 0; y <= m_maxGridPos.y; y++)
	{
		for (size_t x = 0; x <= m_maxGridPos.x; x++)
		{
			Tile& tile = m_tiles[y][x];
			tile.inPath = false;
			tile.visited = false;
			tile.cameFrom = nullptr;
		}
	}

	m_doPrint = true;
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
	if (m_tiles[m_cursorPos.y][m_cursorPos.x].inPath) 
		CalculatePath();
	else 
		ClearPath();
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