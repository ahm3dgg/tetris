#include "tetris.h" 
#include "block.h"
#include "colors.h"
#include "config.h"
#include "draw.h"
#include <cassert>
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
    
    t.bgMusic = LoadMusicStream("assets/music/bg_music.mp3");
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
        auto Row = static_cast<int>(blockPos.x + cells[i].x);
        auto Col = static_cast<int>(blockPos.y + cells[i].y); 
        
        if(t.grid[Row + 1][Col].filled)
        {   
            if(t.bgMusicState != FastPitch && Row <= 8)
            { 
                SeekMusicStream(t.bgMusic, 0);
                SetMusicPitch(t.bgMusic, 2.0f);
                t.bgMusicState = FastPitch; 
            }
            
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

void ShowPreview(const Tetris& t)
{
    // Go From RP -> End 
    // Go From 0 -> WP
    
    int boundingBoxWidth = 4;
    int boundingBoxHeight = 4; 
    int startX = ColCount + 5;
    int x = 0;
    
    for(int i = t.preview_RP; i < t.previewCount; i++, x++)
    {
        int Row = x * boundingBoxHeight + 1;
        
        auto block = t.preview[i];
        auto cells = block.cells[0];
        auto colors = block.colors; 
        
        for(int i = 0; i < 4; i++)
        {
            int cellX = cells[i].y;
            int cellY = cells[i].x;
            
            DrawCell(cellY + Row, cellX + startX, colors[0], colors[1], colors[2]);
        }
    }
    
    for(int i = 0; i < t.preview_WP; i++, x++)
    {       
        int Row = x * boundingBoxHeight + 1;
        
        auto block = t.preview[i];
        auto cells = block.cells[0];
        auto colors = block.colors; 
        
        for(int i = 0; i < 4; i++)  
        {
            int cellX = cells[i].y;
            int cellY = cells[i].x;
            
            DrawCell(cellY + Row, cellX + startX, colors[0], colors[1], colors[2]);
        }
    }
}

void ShowUI(const Tetris& t)
{
    const char* scoreText = TextFormat("Score: %d", t.score);
    const char* levelText = TextFormat("Level: %d", t.level);
    
    DrawText(scoreText, 15 * CellWidth, 20 * CellHeight, 20, WHITE);
    DrawText(levelText, 15 * CellWidth, 21 * CellHeight, 20, WHITE);
    
    // DrawText("Hello, World", 0, 0, 40, YELLOW);
    
    // ShowNextBlock(t.nextBlock);
    ShowPreview(t);
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

void ShiftLines(Tetris& t)
{
    int targetRow = RowCount;
    
    for(int row = RowCount; row >= 1; row--)
    {
        if(!t.linesToClear[row])
        {
            for(int col = 1; col <= ColCount; col++)
            {
                t.grid[targetRow][col] = t.grid[row][col];
            }
            
            targetRow--;
        }
    }
    
    while(targetRow >= 1)
    {
        for(int col = 1; col <= ColCount; col++)
        {
            t.grid[targetRow][col].filled = false; 
            memset(t.grid[targetRow][col].colors, 0, 3*sizeof(Color));
        }
        
        targetRow--;
    }
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
            if(!rowIsFilled) break;
        }
        
        if(rowIsFilled) completedLines++;
        
        t.linesToClear[row] = rowIsFilled;
    }
    
    if(completedLines > 0)
    {   
        t.animating = true;
        t.anim.kind = TetrisAnimationKind::LineClear;
        t.anim.lineClear = {
            .lastTime = GetTime(),
            .frameTime = 100, 
            .col_l = ColCount / 2, 
            .col_r = (ColCount / 2) + 1,
        };
        
        t.completedLinesPerLevel += completedLines; 
        
        if(t.completedLinesPerLevel >= 10)
        {
            t.level++;
            t.completedLinesPerLevel = 0;
        }
        
        t.score += CalculateScore(completedLines, t.level);
        
        TetrisPlaySFX(t, SFX_LineClear);
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
    TetrisFillPreview(t);
    
    
    t.gameover = false; 
    t.currentBlockstate = Block_Unlocked;
    SetMusicPitch(t.bgMusic, 1.0f);
    t.bgMusicState = NormalPitch;
    t.anim = {};
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
    PlaySound(t.sfx[sfx]);
}

void TetrisDoAnimation(Tetris& t)
{   
    if(!t.animating) return;
    
    if( t.anim.kind == TetrisAnimationKind::Gameover )
    {
        auto& gameover = t.anim.gameOver; 
        
        if(gameover.row == 1)
        {
            TetrisPlaySFX(t, SFX_Gameover);
        }
        
        
        double timeElapsed = (GetTime() - gameover.lastTime) * 1000;
        
        if( t.animating && timeElapsed >= gameover.frameTime)
        {       
            for(int col = 1; col <= ColCount; col++)
            {
                t.grid[gameover.row][col].filled = true;
                t.grid[gameover.row][col].colors[0] = RED;
                t.grid[gameover.row][col].colors[1] = YELLOW;
                t.grid[gameover.row][col].colors[2] = RED;
            }
            
            gameover.row++;
            gameover.lastTime = GetTime();
            
            if(gameover.row > RowCount)
                t.animating = false; 
        }
    }
    
    else if( t.anim.kind == TetrisAnimationKind::LineClear)
    {
        auto& lineClear = t.anim.lineClear;
        // if(lineClear.col_l == (ColCount / 2) && lineClear.col_r == (ColCount / 2) + 1)
        // {
        //     TetrisPlaySFX(t, SFX_LineClear);
        // }
        
        for(int row = 1; row <= RowCount; row++)
        {
            if(t.linesToClear[row])
            {
                t.grid[row][lineClear.col_l].filled = false; 
                t.grid[row][lineClear.col_l].colors[0] = {};
                t.grid[row][lineClear.col_l].colors[1] = {};
                t.grid[row][lineClear.col_l].colors[2] = {};
                
                t.grid[row][lineClear.col_r].filled = false; 
                t.grid[row][lineClear.col_r].colors[0] = {};
                t.grid[row][lineClear.col_r].colors[1] = {};
                t.grid[row][lineClear.col_r].colors[2] = {};
            }
        }
      
        
        double timeElapsed = (GetTime() - lineClear.lastTime) * 1000;
        if(timeElapsed >= lineClear.frameTime && t.animating)
        {
            lineClear.lastTime = GetTime();
            lineClear.col_l--;
            lineClear.col_r++;
            
            if(lineClear.col_l < 1 && lineClear.col_r > ColCount)
            {
                t.animating = false;
                ShiftLines(t);
            }
        }
    }
}

void TetrisInsertBlockInPreview(Tetris& t)
{
    if(t.preview_WP > 0)
    {
        Block newBlock = GetRandomBlock();
        Block before = t.preview[t.preview_WP - 1];  
        
        while(newBlock.type == before.type)
            newBlock = GetRandomBlock();
        
        t.preview[t.preview_WP++] = newBlock;
    }
    else
    { 
        t.preview[t.preview_WP++] = GetRandomBlock();
    }
    
    if(t.preview_WP == t.previewCount)
        t.preview_WP = 0;
}   

void TetrisFillPreview(Tetris& t)
{   
    for(int i = 0; i < t.previewCount; i++)
        TetrisInsertBlockInPreview(t);
}

Block TetrisGetBlockFromPreview(Tetris& t)
{
    Block block = t.preview[t.preview_RP];
    t.preview_RP++; 
    
    if(t.preview_RP > t.preview_WP)
        TetrisInsertBlockInPreview(t);
    
    if(t.preview_RP == t.previewCount)
        t.preview_RP = 0; 
    
    return block;
}

void TetrisRun(Tetris& t)
{
    t.previewCount = 4; 
    
    TetrisFillPreview(t);
    
    t.currentBlock = GetRandomBlock();
    
    PlayMusicStream(t.bgMusic);
    
    t.currentBlockstate = Block_Unlocked; 
    t.sfx[SFX_LineClear] = LoadSound("assets/sfx/line_clear.mp3");
    t.sfx[SFX_Gameover] = LoadSound("assets/sfx/game_over.mp3");
    
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        HandleInput(t);
        
        TetrisDoAnimation(t);
        
        if(!t.gameover)
        {
            UpdateMusicStream(t.bgMusic);
            
            if(HasIntervalPassed(t.lastUpdate, 150))
            {
                if(IsBlockAllowedToMove(t, t.currentBlock))
                {
                    MoveBlock(t.currentBlock);
                }
                
                t.currentBlock.velocity = {1, 0};
            }
            
            if(IsCurrentBlockPlaced(t))
            {
                //if(t.currentBlockstate == Block_Unlocked)
                //{
                //    t.currentBlockstate = Block_Locked;
                //    t.lockStartTime = GetTime();
                //}
                //else 
                //{
                double timeElapsedSinceLock = (GetTime() - t.lockStartTime) * 1000.0f;
                
                //    if(t.currentBlockstate == Block_Locked && timeElapsedSinceLock >= LockDelay) 
                {
                    auto cells = t.currentBlock.cells[t.currentBlock.rotationIndex];
                    
                    for(int cell = 0; cell < 4; cell++)
                    {       
                        int cellRowIndex = t.currentBlock.position.x + cells[cell].x;
                        int cellColIndex = t.currentBlock.position.y + cells[cell].y;
                        
                        memcpy(t.grid[cellRowIndex][cellColIndex].colors, t.currentBlock.colors, 3*sizeof(Color));
                        
                        t.grid[cellRowIndex][cellColIndex].filled = true;
                    }
                    
                    // t.currentBlock = t.nextBlock;
                    // t.nextBlock = GetRandomBlock(); 
                    
                    t.currentBlock = TetrisGetBlockFromPreview(t);
                    
                    if(!IsBlockAllowedToMove(t, t.currentBlock))
                    {
                        t.gameover = true;
                        t.animating = true;
                        t.anim.kind = TetrisAnimationKind::Gameover;
                        t.anim.gameOver = {
                            .lastTime = GetTime(),
                            .frameTime = 150,
                            .row = 1,
                        };
                        goto draw;
                    }
                    
                    t.currentBlockstate = Block_Unlocked;
                    // }
                    
                    CheckCompletedLines(t);
                }
            }
        }
        
        draw:
        ShowUI(t); 
        
        
        DrawBlock(t.currentBlock);
        DrawGrid(t);
        
        // if(t.gameover)
        // {
        //     DrawText("Game Over", 5 * CellWidth, 10 * CellHeight, 30, YELLOW);
        // }
        
        EndDrawing();
    }
}
