#include "GridV2.h"

int main()
{
    GridV2 grid = GridV2(115, 24, PathAlgo::A_STAR);
    grid.Loop();
}