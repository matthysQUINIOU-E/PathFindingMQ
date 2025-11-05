#include "GridV2.h"
#include "Ascii.hpp"
#include "Algo.hpp"
#include <Windows.h>
#include <conio.h>

GridV2::GridV2(int width, int height, Position characterPos, int obstacleNumber, PathAlgo pathAlgo) {
	m_pathAlgo = pathAlgo;
	m_characterPos = characterPos;
	m_cursorPos = characterPos;

	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	m_maxGridPos = { width - 1, height - 1 };

	for (size_t y = 0; y < height; y++)
	{
		m_tiles.push_back(std::vector<Tile>());
		for (size_t x = 0; x < width; x++)
		{
			Tile newTile = { {x,y}, true, false, false, -1 };
			m_tiles[y].push_back(newTile);
		}
	}

	GenerateObstacles(obstacleNumber);
}

void GridV2::PrintGrid() {
	system("cls");
	for (int y = 0; y < m_tiles.size(); y++)
	{
		for (int x = 0; x < m_tiles[y].size(); x++)
		{
			Tile& tile = m_tiles[y][x];

			std::cout << "|";
			if (x == m_characterPos.x && y == m_characterPos.y) {
				std::cout << '0';
			}
			else if (x == m_cursorPos.x && y == m_cursorPos.y) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
				std::cout << 'X';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else if (tile.inPath) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
				std::cout << ' ';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else if (tile.walkable) {
				std::cout << ' ';
			}
			else {
				std::cout << '#';
			}
		}
		std::cout << "|" << std::endl;
	}
	m_doPrint = false;
}

void GridV2::Loop() {
	while (true)
	{
		if (m_doPrint)
			PrintGrid();
		HandleInput();
	}
}


void GridV2::HandleInput() {
	auto input = _getch();
	switch (input)
	{
	case Ascii::ENTER:
		if (!CheckPathExist())
			break;
		m_characterPos = m_cursorPos;
		ClearPath();
		m_doPrint = true;
		break;
	case Ascii::W:
	case Ascii::Z:
		m_cursorPos.y--;
		if (m_cursorPos.y < 0)
			m_cursorPos.y = m_tiles.size() - 1;
		CalculatePath();
		break;
	case Ascii::A:
	case Ascii::Q:
		m_cursorPos.x--;
		if (m_cursorPos.x < 0)
			m_cursorPos.x = m_tiles[0].size() - 1;
		CalculatePath();
		break;
	case Ascii::S:
		m_cursorPos.y++;
		m_cursorPos.y %= m_tiles.size();
		CalculatePath();
		break;
	case Ascii::D:
		m_cursorPos.x++;
		m_cursorPos.x %= m_tiles[0].size();
		CalculatePath();
		break;
	default:
		break;
	}
}



void GridV2::CalculatePath() {
	ClearPath();
	m_doPrint = true;
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

void GridV2::GenerateObstacles(int obstacleNumber) {
	int width = m_tiles[0].size();
	int height = m_tiles.size();

	while (obstacleNumber > 0) {
		int x = std::rand() % width;
		int y = std::rand() % height;
		Tile& tile = m_tiles[y][x];
		if (tile.walkable && m_characterPos != tile.position) {
			tile.walkable = false;
			obstacleNumber--;
		}
	}
}

void GridV2::ClearPath() {
	for (size_t y = 0; y < m_tiles.size(); y++)
	{
		for (size_t x = 0; x < m_tiles[y].size(); x++)
		{
			Tile& tile = m_tiles[y][x];
			tile.inPath = false;
			tile.visited = false;
			tile.cameFrom = nullptr;
		}
	}
}

bool GridV2::CheckPathExist() {
	return m_tiles[m_cursorPos.y][m_cursorPos.x].inPath;
}