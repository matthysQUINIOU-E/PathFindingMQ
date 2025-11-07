#include "Grid.h"

int main()
{
    Grid grid = Grid(100, 25, PathAlgo::A_STAR);
    // Grid grid = Grid(200, 50, PathAlgo::A_STAR);
    grid.Loop();
}