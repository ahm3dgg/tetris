#include "config.h"
#include "colors.h"
#include "draw.h"
#include "tetris.h"
#include "block.h"

#include "draw.cpp"
#include "block.cpp"
#include "tetris.cpp"

int WinMain(void* hInstance, void* hPrevInstance, char* lpCmdLine, int nShowCmd)
{
    Tetris t = TetrisInit();
    TetrisRun(t);
}