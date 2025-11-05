#pragma once
#include "Position.hpp"
#include <vector>
#include <algorithm>

struct PathHelper2
{
    Position at;
    Position to;
    std::vector<Position>& obstacles;
    int gridXSize;
    int gridYSize;

    std::vector<Position> moved;

    static std::vector<Position> visiteds;
    static std::vector<int> visitedsPathLentgh;

    static void ClearVisited() {
        visiteds.clear();
        visitedsPathLentgh.clear();
    }

    std::vector<Position> Search() {
        if (Visited())
            return {};

        moved.push_back(at);

        if (at.x == to.x && at.y == to.y) return moved;

        std::vector<Position> neighbor = at.GetNeighbor();
        std::sort(neighbor.begin(), neighbor.end(),
            [this](Position& a, Position& b) {
                return a.CalculateManhattan(to) < b.CalculateManhattan(to);
            });

        std::vector<Position> bestSearch;
        int bestManhattan = -1;

        for (size_t i = 0; i < neighbor.size(); i++)
        {
            Position newAt = neighbor[i];

            int manhattan = newAt.CalculateManhattan(to);
            if (bestManhattan != -1 && bestManhattan < manhattan)
                return bestSearch;

            if (newAt.x < 0 || newAt.y < 0 || newAt.x >= gridXSize || newAt.y >= gridYSize || IsInVectorPosition(obstacles, newAt))
                continue;

            PathHelper2 newHelper = { newAt, to, obstacles, gridXSize, gridYSize, moved };
            std::vector<Position> newSearch = newHelper.Search();

            if (!newSearch.empty()) {
                if (bestManhattan == -1)
                {
                    bestSearch = newSearch;
                    bestManhattan = manhattan;
                }
                else if (bestSearch.size() > newSearch.size()) {
                    bestSearch = newSearch;
                    bestManhattan = manhattan;
                }
            }

        }
        if (bestManhattan != -1)
            return bestSearch;
        return {};
    }

private:
    const bool Visited() const {
        for (size_t i = 0; i < PathHelper2::visiteds.size(); i++)
        {
            Position visited = PathHelper2::visiteds[i];
            if (visited.x == at.x && visited.y == at.y) {
                if (moved.size() >= visitedsPathLentgh[i]) {
                    return true;
                }
                PathHelper2::visiteds[i] = at;
                PathHelper2::visitedsPathLentgh[i] = moved.size();
                return false;
            }
        }
        PathHelper2::visiteds.push_back(at);
        PathHelper2::visitedsPathLentgh.push_back(moved.size());
        return false;
    }

    bool IsInVectorPosition(std::vector<Position> vector, Position position) {
        for (size_t i = 0; i < vector.size(); i++)
        {
            Position pathPosition = vector[i];
            if (pathPosition.x == position.x && pathPosition.y == position.y)
                return true;
        }
        return false;
    }
};