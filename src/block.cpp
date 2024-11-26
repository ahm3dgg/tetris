#include "block.h"
#include "config.h"
#include "draw.h"
#include "colors.h"
#include <cstdlib>
#include <raymath.h>

void DrawBlock(const Block& block)
{
    auto cells = block.cells[block.rotationIndex];
    for(int i = 0; i < 4; i++)
    {
        int cellRowIndex = block.position.x + cells[i].x;
        int cellColIndex = block.position.y + cells[i].y;
        DrawCell(cellRowIndex, cellColIndex, block.colors[0], block.colors[1], block.colors[2]);
    }        
}

bool IsBlockOutside(const Block& block, int row, int col)
{  
    int gridRow = block.position.x + row;
    int gridCol = block.position.y + col;
    
    bool inBounds = (gridCol >= 1 && gridCol <= ColCount) && 
                    (gridRow >= 1 && gridRow <= RowCount);

    return !inBounds;
}

bool CanRotate(const Block& block)
{
    auto nextRotationCells = block.cells[(block.rotationIndex + 1) % 4];
    
    for(int i = 0; i < 4; i++)
    {
        // Check each cell in the new rotation
        int gridRow = block.position.x + nextRotationCells[i].x;
        int gridCol = block.position.y + nextRotationCells[i].y;
        
        // If any cell is outside, rotation is not possible
        if(IsBlockOutside(block, nextRotationCells[i].x, nextRotationCells[i].y))
        {
            return false;
        }
    }
    return true;
}

void RotateBlock(Block& block)
{
    if(block.type == O_Block) return;

    if(CanRotate(block))
    {
        block.rotationIndex = (block.rotationIndex + 1) % 4;
        return;
    }

    Vector2 wallKickOffsets[]
    {
        {0,  -1},  
        {0,  1},  
        {-1, 0},
        {1,  0}  
    };

    for(Vector2 offset : wallKickOffsets)
    {
        block.position.x += offset.x;
        block.position.y += offset.y;

        if(CanRotate(block))
        {
            block.rotationIndex = (block.rotationIndex + 1) % 4;
            return;
        }

        block.position.x -= offset.x;
        block.position.y -= offset.y;
    }
}

void MoveBlock(Block& block)
{
    bool outside = false;
    auto cells = block.cells[block.rotationIndex];
   
    for(int i = 0; i < 4; i++)
    {
        if(IsBlockOutside(block, cells[i].x + block.velocity.x, cells[i].y + block.velocity.y))
        {      
            outside = true;
            break;  
        }
    }
    
    if(!outside)
    {
        block.position.x += block.velocity.x;
        block.position.y += block.velocity.y;
    }
}

Block GetRandomBlock()
{
    int random = rand() % 7;
    
    switch(random)
    {
        case 0: return Create_IBlock();
        case 1: return Create_LBlock();
        case 2: return Create_JBlock();
        case 3: return Create_OBlock();
        case 4: return Create_SBlock();
        case 5: return Create_TBlock();
        case 6: return Create_ZBlock();
        default: return {};
    }
}

Block Create_LBlock()
{
    Block LBlock 
    {
        .type = L_Block,
        .rotationIndex = 0,
        .colors = 
        {
            blue0, 
            blue1, 
            blue2,
        },
        .position = {1,4},
        .cells = 
        {
            {
                {0, 0}, 
                {1, 0}, 
                {1, 1}, 
                {1, 2},
            }, 

            {
                {0, 1}, 
                {0, 2},
                {1, 1},
                {2, 1},
            }, 
            {
                {1, 0}, 
                {1, 1},
                {1, 2},
                {2, 2},
            }, 
            {
                {0, 1}, 
                {1, 1},
                {2, 0},
                {2, 1},
            },
        }
    };

    return LBlock;
}

Block Create_IBlock()
{
    Block IBlock 
    {
        .type = I_Block,
        .rotationIndex = 0,
        .colors = 
        {
            cyan0, 
            cyan1, 
            cyan2,
        },
        .position = {1,4},
        .cells = 
        {
            {
                {1, 0}, 
                {1, 1}, 
                {1, 2},
                {1, 3}
            }, 

            {
                {0, 2}, 
                {1, 2}, 
                {2, 2}, 
                {3, 2},
            }, 

            {
                {2, 0},
                {2, 1}, 
                {2, 2}, 
                {2, 3},
            }, 

            {
                {0, 1}, 
                {1, 1}, 
                {2, 1},
                {3, 1}
            },
        }
    };

    return IBlock;
}

Block Create_JBlock()
{
    Block JBlock 
    {
        .type = J_Block,
        .rotationIndex = 0,
        .colors = 
        {
            orange0, 
            orange1, 
            orange2,
        },
        .position = {1,4},
        .cells = 
        {
            {
                {0, 2}, 
                {1, 0}, 
                {1, 1},
                {1, 2}
            }, 

            {
                {0, 1}, 
                {1, 1},
                {2, 1},
                {2, 2},
            }, 

            {
                {1, 0}, 
                {1, 1},
                {1, 2},
                {2, 0}
            }, 

            {
                {0, 0}, 
                {0, 1},
                {1, 1},
                {2, 1},
            },
        }
    };

    return JBlock;
}

Block Create_OBlock()
{
    Block OBlock 
    {
        .type = O_Block,
        .rotationIndex = 0,
        .colors = 
        {
            yellow0, 
            yellow1, 
            yellow2,
        },
        .position = {1,4},
        .cells = 
        {
            {
                {0, 1}, 
                {0, 2}, 
                {1, 1},
                {1, 2}
            }, 
        }
    };

    return OBlock;
}

Block Create_SBlock()
{
    Block SBlock 
    {
        .type = S_Block,
        .rotationIndex = 0,
        .colors = 
        {
            green0, 
            green1, 
            green2,
        },
        .position = {1,4},
        .cells = 
        {
            {
                {0, 1}, 
                {0, 2},
                {1, 0}, 
                {1, 1},
            }, 

            {
                {0, 1},
                {1, 1}, 
                {1, 2}, 
                {2, 2},
            }, 

            {
                {1, 1}, 
                {1, 2}, 
                {2, 0}, 
                {2, 1},
            }, 

            {
                {0, 0}, 
                {1, 0}, 
                {1, 1},
                {2, 1},
            },
        }
    };

    return SBlock;
}

Block Create_TBlock()
{   
    Block TBlock 
    {
        .type = T_Block,
        .rotationIndex = 0,
        .colors = 
        {
            purple0, 
            purple1, 
            purple2,
        }, 
        .position = {1,4},
        .cells = 
        {
            {
                { 0, 1 }, 
                { 1, 0 }, 
                { 1, 1 }, 
                { 1, 2 }
            }, 

            {
                { 0, 1 }, 
                { 1, 1 }, 
                { 1, 2 }, 
                { 2, 1 }
            }, 

            {
                { 1, 0 }, 
                { 1, 1 }, 
                { 1, 2 }, 
                { 2, 1 }
            }, 

            {
                { 0, 1 }, 
                { 1, 0 }, 
                { 1, 1 }, 
                { 2, 1 }
            },
        }
    };

    return TBlock;
}

Block Create_ZBlock()
{
    Block ZBlock 
    {
        .type = Z_Block,
        .rotationIndex = 0,
        .colors = 
        {
            red0, 
            red1, 
            red2,
        },
        .position = {1, 4},
        .cells = 
        {
            {
                { 0, 0 }, 
                { 0, 1 }, 
                { 1, 1 }, 
                { 1, 2 }
            }, 

            {
                { 0, 2 }, 
                { 1, 1 }, 
                { 1, 2 }, 
                { 2, 1 }
            }, 

            {
                { 1, 0 }, 
                { 1, 1 }, 
                { 2, 1 }, 
                { 2, 2 }
            }, 

            {
                { 0, 1 }, 
                { 1, 0 }, 
                { 1, 1 }, 
                { 2, 0 }
            },
        }
    };

    return ZBlock;
}