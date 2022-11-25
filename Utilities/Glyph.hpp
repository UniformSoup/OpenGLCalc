#pragma once

#include "Resource.hpp"

class Font;

struct Glyph
{
    int index;
    int x0, y0, x1, y1;
    float s0, t0, s1, t1;
    int advanceWidth, leftSideBearing;
};