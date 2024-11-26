#pragma once

#include "block.h"
#include "config.h"

struct GridCell 
{
    bool filled;
    Color colors[3];
};

struct Tetris 
{
    Block currentBlock;
    Block nextBlock;
    int score;
    int level;
    int completedLinesPerLevel;
    float lastUpdate;
    Music bgMusic;
    GridCell grid[RowCount + 2][ColCount + 2];
};

void InitGrid(Tetris& t);
void DrawGrid(Tetris& t);

bool HasIntervalPassed(float& lastTime, float interval);

Tetris TetrisInit();
void TetrisRun(Tetris& t);