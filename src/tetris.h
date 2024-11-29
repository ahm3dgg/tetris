#pragma once

#include "block.h"
#include "config.h"

struct GridCell 
{
    bool filled;
    Color colors[3];
};

constexpr float LockDelay = 500;
constexpr int RotationCap = 3;

enum BlockState 
{
    None = 0, 
    Block_Locked, 
    Block_Unlocked
};

enum TetrisSFX
{
    SFX_LineClear = 0,
};

struct LineClearAnim 
{
    double lastTime;
    double frameTime; 

    int col_l;
    int col_r; 
};

struct Tetris 
{
    GridCell grid[RowCount + 2][ColCount + 2]; 

    Block currentBlock;
    Block nextBlock;
    BlockState currentBlockstate;

    int score;
    int level;
    int completedLinesPerLevel;
    int rotationsCount;
    
    float lockStartTime;
    float lastUpdate; 

    bool gameover;

    bool linesToClear[RowCount];

    bool animating;
    LineClearAnim currentAnim;

    Sound sfx[5];

    Music bgMusic; 
};

void InitGrid(Tetris& t);
void DrawGrid(Tetris& t);

bool HasIntervalPassed(float& lastTime, float interval);

Tetris TetrisInit();
void TetrisRun(Tetris& t);