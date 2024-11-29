#include "tetris.h"
#include "block.h"
#include "colors.h"
#include "config.h"
#include "draw.h"
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <time.h>
#include <raylib.h>
#include <raymath.h>

void InitGrid(Tetris& t)
{   
    for(int row = 0; row < RowCount + 2; row++)
    {
        for(int col = 0; col < ColCount + 2; col++)
        {
            t.grid[row][col].colors[0] = {};
            t.grid[row][col].colors[1] = {};
            t.grid[row][col].colors[2] = {};

            t.grid[row][col].filled = false; 
        }
    }

    for(int col = 0; col < ColCount + 2; col++)
    {
        t.grid[0][col].colors[0] = darkGray;
        t.grid[0][col].colors[1] =  GRAY; 
        t.grid[0][col].colors[2] = lightGray;
        t.grid[0][col].filled = true;

        
        t.grid[RowCount + 1][col].colors[0] = darkGray;
        t.grid[RowCount + 1][col].colors[1] =  GRAY; 
        t.grid[RowCount + 1][col].colors[2] = lightGray;
        t.grid[RowCount + 1][col].filled = true;
    }
    
    for(int row = 0; row < RowCount + 2; row++)
    {   
        t.grid[row][0].colors[0] = darkGray;
        t.grid[row][0].colors[1] =  GRAY; 
        t.grid[row][0].colors[2] = lightGray;
        t.grid[row][0].filled = true;

        
        t.grid[row][ColCount + 1].colors[0] = darkGray;
        t.grid[row][ColCount + 1].colors[1] =  GRAY; 
        t.grid[row][ColCount + 1].colors[2] = lightGray;
        t.grid[row][ColCount + 1].filled = true;
    }
}

void DrawGrid(Tetris& t)
{
    for(int row = 0; row < RowCount + 2; row++)
    {
        for(int col = 0; col < ColCount + 2; col++)
        {
            auto colors = t.grid[row][col].colors; 

            if(t.grid[row][col].filled)
            {
                DrawCell(row, col, colors[0], colors[1], colors[2]);
            }
        }
    }
}

Tetris TetrisInit()
{
    Tetris t{};
    
    InitAudioDevice();
    InitWindow(WindowWidth, WindowHeight, "Tetris");
    InitGrid(t);
    srand(time(NULL));
    SetTargetFPS(60);

    t.bgMusic = LoadMusicStream("tetris-bg-music.wav");
    t.gameover = false;

    return t;
}

bool HasIntervalPassed(float& lastTime, float interval)
{
    float currentTime = GetTime() * 1000.0f;

    if(currentTime - lastTime >= interval)
    {
        lastTime = currentTime;
        return true;
    }

    return false; 
}

bool IsCurrentBlockPlaced(Tetris& t)
{
    auto cells = t.currentBlock.cells[t.currentBlock.rotationIndex];
    auto blockPos = t.currentBlock.position;

    for(int i = 0; i < 4; i++)
    {
        auto currentCellX = static_cast<int>(blockPos.x + cells[i].x);
        auto currentCellY = static_cast<int>(blockPos.y + cells[i].y); 

        if(t.grid[currentCellX + 1][currentCellY].filled)
        {
            return true;
        }
    }

    return false; 
}

bool IsRowFilled(Tetris& t, int row)
{
    for(int col = 1; col <= ColCount; col++)
    {
        if(!t.grid[row][col].filled)
            return false; 
    }

    return true;
}

int CalculateScore(int clearedLines, int level)
{
    if(clearedLines == 1)
        return 40 * (level + 1);
    else if(clearedLines == 2)
        return 100 * (level + 1); 
    else if(clearedLines == 3)
        return 300 * (level + 1);
    else if(clearedLines == 4)
        return 1200 * (level + 1);
    else 
        return 0;
}

void ShiftLines(Tetris& t)
{
    int completedLines = 0;
    int targetRow = RowCount;

    for (int row = RowCount; row >= 1; row--)
    {
        if (!IsRowFilled(t, row))
        {
            if (completedLines > 0)
            {
                for (int col = 1; col <= ColCount; col++)
                {
                    t.grid[targetRow][col] = t.grid[row][col];

                    t.grid[row][col].filled = false;
                    t.grid[row][col].colors[0] = {};
                    t.grid[row][col].colors[1] = {};
                    t.grid[row][col].colors[2] = {};
                }
            }
            
            targetRow--;
        }
        else
        {   
            completedLines++;
        }
    }

    if(completedLines > 0)
    {   
        t.completedLinesPerLevel += completedLines; 

        if(t.completedLinesPerLevel >= 10)
        {
            t.level++;
            t.completedLinesPerLevel = 0;
        }

        t.score += CalculateScore(completedLines, t.level);
    }
}

void ShowNextBlock(const Block& nextBlock)
{
    DrawText("Next Block: ", 15 * CellWidth, 1 * CellHeight, 20, WHITE); 

    DrawRectangleLines(15 * CellWidth, 3*CellHeight, 5*CellWidth, 4*CellHeight, YELLOW); 

    auto cells = nextBlock.cells[0];
    auto colors = nextBlock.colors; 

    for(int i = 0; i < 4; i++)  
    {
        int rowIndex = 0, colIndex = 0;   

        rowIndex = 4 + cells[i].x; 
        colIndex = 16 + cells[i].y;
        
        DrawCell(rowIndex, colIndex, colors[0], colors[1], colors[2]);
    }
}

void ShowUI(const Tetris& t)
{
    const char* scoreText = TextFormat("Score: %d", t.score);
    const char* levelText = TextFormat("Level: %d", t.level);

    DrawText(scoreText, 15 * CellWidth, 20 * CellHeight, 20, WHITE);
    DrawText(levelText, 15 * CellWidth, 21 * CellHeight, 20, WHITE);
    
    ShowNextBlock(t.nextBlock);
}

bool IsBlockOutside(const Block& block, int row, int col)
{  
    int gridRow = block.position.x + row;
    int gridCol = block.position.y + col;
    
    bool inBounds = (gridCol >= 1 && gridCol <= ColCount) && 
                    (gridRow >= 1 && gridRow <= RowCount);

    return !inBounds;
}

bool IsBlockAllowedToMove(Tetris& t, const Block& block)
{  
    auto cells = block.cells[block.rotationIndex];

    for(int i = 0; i < 4; i++)
    {
        if(IsBlockOutside(block, block.velocity.x + cells[i].x, block.velocity.y + cells[i].y))
            return false; 

        int nextCellRow = block.position.x + block.velocity.x + cells[i].x;
        int nextCellCol = block.position.y + block.velocity.y + cells[i].y;
        
        bool overlaps = t.grid[nextCellRow][nextCellCol].filled;
        if(overlaps) return false;
    }

    return true;
}

void CheckCompletedLines(Tetris& t)
{
    int completedLines = 0; 

    for(int row = 1; row <= RowCount; row++)
    {
        bool rowIsFilled = true;  

        for(int col = 1; col <= ColCount; col++)
        {
            rowIsFilled &= t.grid[row][col].filled;
        }

        if(rowIsFilled) completedLines++;

        t.linesToClear[row] = rowIsFilled;
    }

    if(completedLines > 0)
    {   
        t.currentAnim =
        {
            .lastTime = GetTime(), 
            .frameTime = 150,
            .col_l = ColCount / 2,
            .col_r = (ColCount / 2) + 1, 
        };

        t.animating = true;
        
        t.completedLinesPerLevel += completedLines; 

        if(t.completedLinesPerLevel >= 10)
        {
            t.level++;
            t.completedLinesPerLevel = 0;
        }

        t.score += CalculateScore(completedLines, t.level);
    }
}

bool IsStandardRotationAllowed(Tetris& t, Block& block)
{
     auto nextRotationCells = block.cells[(block.rotationIndex + 1) % 4];
    
    for(int i = 0; i < 4; i++)
    {
        int gridRow = block.position.x + nextRotationCells[i].x;
        int gridCol = block.position.y + nextRotationCells[i].y;
        
        if(IsBlockOutside(block, nextRotationCells[i].x, nextRotationCells[i].y))
        {
            return false;
        }

        bool overlaps = t.grid[gridRow][gridCol].filled;
        if(overlaps) return false; 
    }

    return true;
}

bool IsBlockAllowedToRotate(Tetris& t, Block& block)
{
    if(IsStandardRotationAllowed(t, block))
        return true;

    Vector2 wallKickOffsets[]
    {
        {0,  -1},  
        {0,  1},  
        {-1, 0},
        {1,  0}  
    };

    for(Vector2 offset : wallKickOffsets)
    {
        block.position = Vector2Add(block.position, offset);

        if(IsStandardRotationAllowed(t, block))
            return true;

        block.position = Vector2Subtract(block.position, offset);
    }

    return false; 
}

void ResetGame(Tetris& t)
{
    t.level = 0;
    t.completedLinesPerLevel = 0;
    t.score = 0;

    for(int row = 1; row <= RowCount; row++)
    {
        for(int col = 1; col <= ColCount; col++)
        {
            t.grid[row][col].colors[0] = {};
            t.grid[row][col].colors[1] = {};
            t.grid[row][col].colors[2] = {};

            t.grid[row][col].filled = false; 
        }
    }

    
    SeekMusicStream(t.bgMusic, 0);
    t.currentBlock = GetRandomBlock();
    t.nextBlock = GetRandomBlock(); 
    while(t.currentBlock.type == t.nextBlock.type)
        t.nextBlock = GetRandomBlock();

    t.gameover = false; 
    t.currentBlockstate = Block_Unlocked;
}

void HandleInput(Tetris& t)
{
    if(IsKeyPressed(KEY_UP))
    {
        if(IsBlockAllowedToRotate(t, t.currentBlock))
        {
            RotateBlock(t.currentBlock);

            if(t.currentBlockstate == Block_Locked && t.rotationsCount == RotationCap)
                t.lockStartTime = GetTime();
            
            t.rotationsCount = (t.rotationsCount + 1) % RotationCap;
        }
    }

    if(IsKeyPressed(KEY_RIGHT))
    {
        t.currentBlock.velocity = {0, 1};
        t.lockStartTime = GetTime();
    }

    if(IsKeyPressed(KEY_LEFT))
    {
        t.currentBlock.velocity = {0, -1};
        t.lockStartTime = GetTime();
    }

    if(IsKeyPressed(KEY_DOWN))
    {
        t.currentBlock.velocity = {2, 0};
        t.lockStartTime = GetTime();
    }

    if(IsKeyPressed(KEY_R))
    {
        ResetGame(t);
    }
}

void TetrisPlaySFX(const Tetris& t, TetrisSFX sfx)
{
    // PlaySound(t.sfx[sfx]);
}

void TetrisRun(Tetris& t)
{
    t.currentBlock = GetRandomBlock();
    t.nextBlock = GetRandomBlock();

    PlayMusicStream(t.bgMusic);

    t.currentBlockstate = Block_Unlocked; 
    t.sfx[SFX_LineClear] = LoadSound("sfx_line_clear.mp3");

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        HandleInput(t);

        if(!t.gameover)
        {
            UpdateMusicStream(t.bgMusic);

            if(HasIntervalPassed(t.lastUpdate, 50))
            {
                if(IsBlockAllowedToMove(t, t.currentBlock))
                {
                    MoveBlock(t.currentBlock);
                }
                
                t.currentBlock.velocity = {1, 0};
            }

            if(IsCurrentBlockPlaced(t))
            {
                if(t.currentBlockstate == Block_Unlocked)
                {
                    t.currentBlockstate = Block_Locked;
                    t.lockStartTime = GetTime();
                }
                else 
                {
                    double timeElapsedSinceLock = (GetTime() - t.lockStartTime) * 1000.0f;

                    if(t.currentBlockstate == Block_Locked && timeElapsedSinceLock >= LockDelay) 
                    {
                        auto cells = t.currentBlock.cells[t.currentBlock.rotationIndex];

                        for(int cell = 0; cell < 4; cell++)
                        {       
                            int cellRowIndex = t.currentBlock.position.x + cells[cell].x;
                            int cellColIndex = t.currentBlock.position.y + cells[cell].y;

                            memcpy(t.grid[cellRowIndex][cellColIndex].colors, t.currentBlock.colors, 3*sizeof(Color));

                            t.grid[cellRowIndex][cellColIndex].filled = true;
                        }

                        t.currentBlock = GetRandomBlock();
                        t.nextBlock = GetRandomBlock();
                       
                        if(!IsBlockAllowedToMove(t, t.currentBlock))
                        {
                            t.gameover = true;
                            goto draw;
                        }

                        ShiftLines(t);
                        t.currentBlockstate = Block_Unlocked;
                    }
                }
            }
        }

draw:
        DrawGrid(t);
        ShowUI(t); 
        DrawBlock(t.currentBlock);

        if(t.gameover)
            DrawText("Game Over", 5 * CellWidth, 10 * CellHeight, 30, YELLOW);

        EndDrawing();
    }
}