#include "GridV2.h"

int main()
{
    GridV2 grid = GridV2(100, 25, PathAlgo::A_STAR);
    // GridV2 grid = GridV2(200, 50, PathAlgo::A_STAR);
    grid.Loop();
}