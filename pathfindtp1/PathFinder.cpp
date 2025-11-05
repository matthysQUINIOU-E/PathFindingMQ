#include "GridV2.h"

int main()
{
    GridV2 grid = GridV2(25, 25, { 12,13 }, 200, PathAlgo::A_STAR);
    grid.Loop();
}