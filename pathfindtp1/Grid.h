/*
#pragma once
#include <vector>
#include "Position.hpp"

class Grid
{
public:
	Grid(int width, int height, Position characterPos, int obstacleNumber);
	void Loop();

private:
	void PrintGrid();
	void HandleInput();
	void ChangeCharacterPos();
	void CalculatePath();
	bool IsInVectorPosition(std::vector<Position> vector, Position position);
	int CalculateManhattan();
	void PrintManhattan();
	void GenerateObstacles(int obstacleNumber);

private:
	std::vector<std::vector<char>> m_grid;
	std::vector<Position> m_path;
	std::vector<Position> m_obstacles;
	Position m_cursorPos;
	Position m_characterPos;
	bool m_doPrint = true;
	
};

*/