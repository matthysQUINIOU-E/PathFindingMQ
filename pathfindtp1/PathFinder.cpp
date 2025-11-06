#include "GridV2.h"

int main()
{
    GridV2 grid = GridV2(3, 3, PathAlgo::A_STAR);
    grid.Loop();
}