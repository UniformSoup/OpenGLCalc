#pragma once

#include "stb_truetype.h"

#include <memory>
#include <string>
#include <vector>

#include "FontAtlas.hpp"

class Font
{
    std::shared_ptr<unsigned char[]> m_filebuffer;
    stbtt_fontinfo m_info;
    int m_ascent, m_descent, m_linegap;

public:
    Font(const char* filepath);

    FontAtlas createAtlas(const std::vector<int>& characters, const unsigned int& font_size, const unsigned int& oversampling = 2);

    int getKerning(const Glyph& left, const Glyph& right) const;
    void getVerticalMetrics(int* ascent, int* descent, int* linegap) const;
};
