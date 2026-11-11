#pragma once

#include "block.h"
#include "config.h"
#include <cstddef>

struct GridCell 
{
    bool filled;
    Color colors[3];
};

constexpr float LockDelay = 300;
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
    SFX_Gameover,
};

struct LineClearAnim 
{
    double lastTime;
    double frameTime; 
    
    int col_l;
    int col_r; 
};

enum class TetrisAnimationKind
{
    None = 0,
    LineClear,
    Gameover,
};

enum BG_Music
{
    NormalPitch = 0, 
    FastPitch,
    LockedPitch,
};

struct TetrisAnimation 
{
    TetrisAnimationKind kind;
    
    union 
    {   
        struct 
        {
            double lastTime;
            double frameTime; 
            int col_l;
            int col_r; 
        } lineClear;
        
        struct 
        {
            double lastTime;
            double frameTime;
            int row;
        }gameOver;
    };
};

struct Tetris 
{
    GridCell grid[RowCount + 2][ColCount + 2]; 
    
    Block currentBlock;
    Block nextBlock;
    BlockState currentBlockstate;
    
    TetrisAnimation anim;
    
    int score;
    int level;
    int completedLinesPerLevel;
    int rotationsCount;
    
    BG_Music bgMusicState;   
    
    float musicPitch;
    
    float lockStartTime;
    float lastUpdate; 
    
    bool gameover;
    
    bool linesToClear[RowCount + 1];
    
    bool animating;
    
    Sound sfx[5];
    
    Block preview[5];
    size_t previewCount;
    size_t preview_RP;
    size_t preview_WP;
    
    Music bgMusic; 
};

void InitGrid(Tetris& t);
void DrawGrid(Tetris& t);

bool HasIntervalPassed(float& lastTime, float interval);

Tetris TetrisInit();
void TetrisRun(Tetris& t);

void TetrisPlaySFX(const Tetris& t, TetrisSFX sfx);
void TetrisFillPreview(Tetris& t);