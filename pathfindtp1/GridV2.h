#pragma once
#include "Tile.h"
#include "PathAlgo.hpp"

class GridV2
{
public:
	GridV2(int width, int height, PathAlgo pathAlgo);
	void Loop();

private:
	void PrintGrid();
	void HandleInput();

	void CalculatePath();
	void ClearPath();

	void ResetMaze();
	void GenerateMaze();

private:
	std::vector<std::vector<Tile>> m_tiles;

	Position m_cursorPos;
	Position m_characterPos;
	Position m_maxGridPos;

	PathAlgo m_pathAlgo;

	bool m_doPrint = true;

};

