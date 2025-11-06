#pragma once
#include "Tile.h"
#include "Node.hpp"
#include "PathAlgo.hpp"
#include <chrono>

class GridV2
{
public:
	GridV2(int width, int height, PathAlgo pathAlgo);
	~GridV2();
	void Loop();

private:
	void PrintGrid();
	void HandleInput();

	void CalculatePath();
	void ClearPath();
	void SwitchPathAlgo();
	const char* GetPathAlgoName();

	void ResetMaze();
	void GenerateMaze();

	void BuildGraph();
	void DeleteNodes();

	void DumbSearch(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos);
	void Bfs(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos);
	void Dfs(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos);
	void AStar(std::vector<Node<Tile>*>& nodes, Position& from, Position& to, Position& maxPos);

	Node<Tile>* GetNodeCharacter();
	Node<Tile>* GetNodeCursor();

private:
	std::vector<std::vector<Tile>> m_tiles;

	Position m_cursorPos;
	Position m_characterPos;
	Position m_maxGridPos;

	PathAlgo m_pathAlgo;

	bool m_doPrint = true;

	std::chrono::microseconds m_algoExecutionTime;

	std::vector<Node<Tile>*> m_graph;
};

