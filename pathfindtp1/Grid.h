#pragma once
#include "Tile.h"
#include "Node.hpp"
#include "PathAlgo.hpp"
#include "Comparator.hpp"
#include <chrono>
#include <queue>
#include <unordered_map>

class Grid
{
public:
	Grid(int width, int height, PathAlgo = PathAlgo::A_STAR);
	~Grid();
	void Loop();

private:
	void PrintGrid();
	void HandleInput();

	void CalculatePath();
	void ClearPath();
	void SwitchPathAlgo();

	void ResetMaze();
	void GenerateMaze();

	void BuildGraph();
	void DeleteNodes();

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

