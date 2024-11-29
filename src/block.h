#pragma once
#include <raylib.h>

enum BlockType
{
    I_Block = 0, 
    L_Block,
    J_Block,
    O_Block, 
    S_Block, 
    T_Block, 
    Z_Block,
};

struct Block 
{
    BlockType type;
    int rotationIndex;
    Color colors[3];
    Vector2 position;
    Vector2 velocity;
    Vector2 cells[4][4];
};

void DrawBlock(const Block& block);
void RotateBlock(Block& block);
void MoveBlock(Block& block);

Block Create_LBlock();
Block Create_IBlock();
Block Create_JBlock();
Block Create_OBlock();
Block Create_SBlock();
Block Create_TBlock();
Block Create_ZBlock();

Block GetRandomBlock();