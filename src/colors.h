#pragma once
#include <raylib.h>

constexpr Color darkGray = {51, 51, 51, 255};
constexpr Color lightGray = {119, 119, 119, 255};

constexpr Color green0 = {0, 151, 0, 255};
constexpr Color green2 = {0, 205, 0, 255};
constexpr Color green1 = {0, 255, 0, 255};

constexpr Color red0 = {152, 0, 11, 255};
constexpr Color red1 = {255, 0, 0, 255};
constexpr Color red2 = {205, 0, 0, 255};

constexpr Color blue0 = {0, 0, 159, 255};
constexpr Color blue1 = {0, 0, 225, 255};
constexpr Color blue2 = {0, 0, 205, 255};

constexpr Color orange0 = {154, 66, 0, 255};
constexpr Color orange1 = {230, 123, 0, 255};
constexpr Color orange2 = {205, 102, 0, 255};

constexpr Color cyan0 = {0, 149, 149, 255};
constexpr Color cyan1 = {0, 250, 250, 255};
constexpr Color cyan2 = {0, 205, 205, 255};

constexpr Color purple0 = {102, 0, 154, 255};
constexpr Color purple1 = {187, 2, 232, 255};
constexpr Color purple2 = {154, 0, 205, 255};

constexpr Color yellow0 = {178, 178, 0, 255};
constexpr Color yellow1 = {238, 236, 0, 255};
constexpr Color yellow2 = {205, 205, 0, 255};

constexpr Color colors[7][3] = 
{
    {red0, red1, red2}, 
    {blue0, blue1, blue2}, 
    {orange0, orange1, orange2}, 
    {cyan0, cyan1, cyan2}, 
    {purple0, purple1, purple2}, 
    {yellow0, yellow1, yellow2},
    {green0, green1, green2},
};