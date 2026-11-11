#include "config.h"
#include "draw.h"
#include <raylib.h>
#include <stdio.h>

void DrawCell(int row, int col, Color backColor, Color triangleColor, Color frontColor)
{
    Vector2 v1 = {(float)col*CellWidth + CellWidth, (float)row*CellHeight};
    Vector2 v2 = {(float)col*CellWidth, (float)row*CellHeight};
    Vector2 v3 = {(float)col*CellWidth, (float)row*CellHeight + CellHeight};

    float smolWidth = 22; 
    float smolHeight = 22;
    
    DrawRectangle(col*CellWidth, row*CellHeight, CellWidth, CellHeight, backColor);
    DrawTriangle(v1, v2, v3, triangleColor);
    
    DrawRectangle(
        (col*CellWidth + (float)CellWidth / 2) - (smolWidth / 2), 
        (row*CellHeight + (float)CellHeight / 2) - (smolHeight / 2), 
        smolWidth, smolHeight, 
        frontColor);
}

// void DrawGrid()
// {
//     for(int col = 0; col < ColCount + 2; col++)
//     {
//         DrawCell(0, col, darkGray, GRAY, lightGray);
//         DrawCell(RowCount + 1, col, darkGray, GRAY, lightGray);
//     }
    
//     for(int row = 0; row < RowCount + 2; row++)
//     {
//         DrawCell(row, 0, darkGray, GRAY, lightGray);
//         DrawCell(row, ColCount + 1, darkGray, GRAY, lightGray);
//     }
// }
