#pragma once
#include "Tile.h"

void DumbSearch(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos);
void Bfs(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos);
void Dfs(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos);
void AStar(std::vector<std::vector<Tile>>& tiles, Position& from, Position& to, Position& maxPos);