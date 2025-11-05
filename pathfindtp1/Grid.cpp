#include "Grid.h"
#include "Ascii.hpp"
#include "PathHelper.hpp"
#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <ctime>
#include "PathHelper2.h"

Grid::Grid(int width, int height, Position characterPos, int obstacleNumber) {
	if (width <= 0) width = 1;
	if (height <= 0) height = 1;

	for (size_t y = 0; y < width; y++)
	{
		m_grid.push_back(std::vector<char>());
		for (size_t x = 0; x < height; x++)
		{
			if (x == characterPos.x && y == characterPos.y)
				m_grid[y].push_back('0');
			else 
				m_grid[y].push_back(' ');
		}
	}

	GenerateObstacles(obstacleNumber);

	m_characterPos = characterPos;
	m_cursorPos = characterPos;
}

void Grid::PrintGrid() {
	system("cls");
	for (int y = 0; y < m_grid.size(); y++)
	{
		for (int x = 0; x < m_grid[y].size(); x++)
		{
			std::cout << "|";
			if (x == m_cursorPos.x && y == m_cursorPos.y) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
				std::cout << ' ';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else if (IsInVectorPosition(m_path,{ x,y })) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);
				std::cout << ' ';
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN);
			}
			else 
				std::cout << m_grid[y][x];
		}
		std::cout << "|" << std::endl;
	}
	PrintManhattan();
	m_doPrint = false;
}

void Grid::Loop() {
	while (true)
	{
		if (m_doPrint)
			PrintGrid();
		HandleInput();
	}
}

void Grid::HandleInput() {
	auto input = _getch();
	switch (input)
	{
	case Ascii::ENTER:
		if (m_path.size() == 0)
			break;
		ChangeCharacterPos();
		m_path.clear();
		PathHelper::ClearVisited();
		m_doPrint = true;
		break;
	case Ascii::W:
	case Ascii::Z:
		m_cursorPos.y--;
		if (m_cursorPos.y < 0)
			m_cursorPos.y = m_grid.size() - 1;
		m_doPrint = true;
		if (IsInVectorPosition(m_obstacles, m_cursorPos)) {
			m_path.clear();
			break;
		}
		CalculatePath();
		break;
	case Ascii::A:
	case Ascii::Q:
		m_cursorPos.x--;
		if (m_cursorPos.x < 0)
			m_cursorPos.x = m_grid[0].size() - 1;
		m_doPrint = true;
		if (IsInVectorPosition(m_obstacles, m_cursorPos)) {
			m_path.clear();
			break;
		}
		CalculatePath();
		break;
	case Ascii::S:
		m_cursorPos.y++;
		m_cursorPos.y %= m_grid.size();
		m_doPrint = true;
		if (IsInVectorPosition(m_obstacles, m_cursorPos)) {
			m_path.clear();
			break;
		}
		CalculatePath();
		break;
	case Ascii::D:
		m_cursorPos.x++;
		m_cursorPos.x %= m_grid[0].size();
		m_doPrint = true;
		if (IsInVectorPosition(m_obstacles, m_cursorPos)) {
			m_path.clear();
			break;
		}
		CalculatePath();
		break;
	default:
		break;
	}
}

void Grid::ChangeCharacterPos() {
	m_grid[m_characterPos.y][m_characterPos.x] = ' ';
	m_grid[m_cursorPos.y][m_cursorPos.x] = '0';
	m_characterPos = m_cursorPos;
}

void Grid::CalculatePath() {
	/*
	PathHelper::SetVisitedFlag();
	PathHelper pathHelper = { m_characterPos, m_cursorPos, m_grid[0].size(), m_grid.size(), m_obstacles};
	m_path = pathHelper.Search((m_grid[0].size() + m_grid.size()) * 2);
	*/
	PathHelper2::ClearVisited();
	PathHelper2 pathHelper = { m_characterPos, m_cursorPos, m_obstacles, m_grid[0].size(), m_grid.size() };
	m_path = pathHelper.Search(); 
}

bool Grid::IsInVectorPosition(std::vector<Position> vector,Position position) {
	for (size_t i = 0; i < vector.size(); i++)
	{
		Position pathPosition = vector[i];
		if (pathPosition.x == position.x && pathPosition.y == position.y)
			return true;
	}
	return false;
}

int Grid::CalculateManhattan() {
	return m_characterPos.CalculateManhattan(m_cursorPos);
}

void Grid::PrintManhattan() {
	std::cout << "Distance Manhattan = " << CalculateManhattan() << std::endl;
}

void Grid::GenerateObstacles(int obstacleNumber) {
	int width = m_grid[0].size();
	int height = m_grid.size();

	while (obstacleNumber > 0) {
		int x = std::rand() % width;
		int y = std::rand() % height;

		if (m_grid[y][x] == ' ') {
			m_grid[y][x] = '#';
			m_obstacles.push_back({ x, y });
			obstacleNumber--;
		}
	}
}
