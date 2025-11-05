#pragma once
#include "Tile.h"
#include "PathAlgo.hpp"

class GridV2
{
public:
	GridV2(int width, int height, Position characterPos, int obstacleNumber, PathAlgo pathAlgo);
	void Loop();

private:
	void PrintGrid();
	void HandleInput();
	void CalculatePath();
	void GenerateObstacles(int obstacleNumber);
	void ClearPath();
	bool CheckPathExist();

private:
	std::vector<std::vector<Tile>> m_tiles;

	Position m_cursorPos;
	Position m_characterPos;
	Position m_maxGridPos;

	PathAlgo m_pathAlgo;

	bool m_doPrint = true;

};

