#include "GridV2.h"

int main()
{
    GridV2 grid = GridV2(10, 10, PathAlgo::A_STAR);
    grid.Loop();
}