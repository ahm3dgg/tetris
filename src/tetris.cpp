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
    for(int col = 1; col < ColCount; col++)
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

void HandleFilledRows(Tetris& t)
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

void HandleInput(Tetris& t)
{
    if(IsKeyPressed(KEY_UP))
        RotateBlock(t.currentBlock);

    if(IsKeyPressed(KEY_RIGHT))
        t.currentBlock.velocity = {0, 1};

    if(IsKeyPressed(KEY_LEFT))
        t.currentBlock.velocity = {0, -1};

    if(IsKeyPressed(KEY_DOWN))
        t.currentBlock.velocity = {2, 0};
}

void TetrisRun(Tetris& t)
{
    t.currentBlock = GetRandomBlock();
    t.nextBlock = GetRandomBlock(); 

    while(t.currentBlock.type == t.nextBlock.type)
        t.nextBlock = GetRandomBlock();

    PlayMusicStream(t.bgMusic);

    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateMusicStream(t.bgMusic);

        HandleInput(t);

        if(HasIntervalPassed(t.lastUpdate, 250))
        {
            MoveBlock(t.currentBlock);
            t.currentBlock.velocity = {1, 0};
        }

        if(IsCurrentBlockPlaced(t))
        {
            auto cells = t.currentBlock.cells[t.currentBlock.rotationIndex];

            for(int cell = 0; cell < 4; cell++)
            {       
                int cellRowIndex = t.currentBlock.position.x + cells[cell].x;
                int cellColIndex = t.currentBlock.position.y + cells[cell].y;

                t.grid[cellRowIndex][cellColIndex].colors[0] = t.currentBlock.colors[0];
                t.grid[cellRowIndex][cellColIndex].colors[1] = t.currentBlock.colors[1];
                t.grid[cellRowIndex][cellColIndex].colors[2] = t.currentBlock.colors[2];

                t.grid[cellRowIndex][cellColIndex].filled = true;
            }

            t.currentBlock = t.nextBlock;
            t.nextBlock = GetRandomBlock();
            while(t.nextBlock.type == t.currentBlock.type)
                t.nextBlock = GetRandomBlock();
            
            HandleFilledRows(t);
        }
        
        DrawGrid(t);
        ShowUI(t); 
        DrawBlock(t.currentBlock);
        EndDrawing();
    }
}