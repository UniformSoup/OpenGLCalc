#define STB_TRUETYPE_IMPLEMENTATION
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#include "Font.hpp"

#include "Glyph.hpp"

#include <fstream>
#include <algorithm>
#include <map>

Font::Font(const char* filepath)
{  
    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if (!file.is_open()) throw std::runtime_error("Could Not Find Font File:\t" + std::string(filepath));

    int size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_filebuffer = std::shared_ptr<unsigned char[]>(new unsigned char[size]);
    file.read((char *) m_filebuffer.get(), size);

    if (!stbtt_InitFont(&m_info, m_filebuffer.get(), 0)) throw std::runtime_error("Failed To Initialise Font.");

    stbtt_GetFontVMetrics(&m_info, &m_ascent, &m_descent, &m_linegap);
}

FontAtlas Font::createAtlas(const std::vector<int>& unique_codepoints, const unsigned int& font_height, const unsigned int& oversampling)
{
    std::vector<stbtt_packedchar> packedchar(unique_codepoints.size());

    const int width = (font_height/2) * unique_codepoints.size() * oversampling;
    const int height = font_height * oversampling;

    unsigned char* pixels = (unsigned char*) calloc(width * height, sizeof(unsigned char));

    {
        stbtt_pack_context pc;
        stbtt_pack_range range;
        range.chardata_for_range = packedchar.data();
        range.array_of_unicode_codepoints = unique_codepoints.data();
        range.first_unicode_codepoint_in_range = 0;
        range.num_chars = unique_codepoints.size();
        range.font_size = font_height;

        stbtt_PackBegin(&pc, pixels, width, height, 0, 1, nullptr);
        stbtt_PackSetOversampling(&pc, oversampling, oversampling);
        stbtt_PackFontRanges(&pc, m_filebuffer.get(), 0, &range, 1);
        stbtt_PackEnd(&pc);
    }

    Texture atlas;
    atlas->bind();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // We store the data with only one Byte
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(pixels);

    std::map<int, Glyph> glyphs;

    for (size_t i = 0; i < unique_codepoints.size(); ++i)
    {
        Glyph g;

        g.index = stbtt_FindGlyphIndex(&m_info, unique_codepoints[i]);
        g.advanceWidth = packedchar[i].xadvance;

        g.x0 = packedchar[i].xoff;
        g.y0 = packedchar[i].yoff;
        g.x1 = packedchar[i].xoff2;
        g.y1 = packedchar[i].yoff2;
        stbtt_GetGlyphBox(&m_info, g.index, &g.x0, &g.y0, &g.x1, &g.y1);
        stbtt_GetGlyphHMetrics(&m_info, g.index, &g.advanceWidth, &g.leftSideBearing);

        g.s0 = packedchar[i].x0 / (float) width;
        g.t0 = packedchar[i].y0 / (float) height;
        g.s1 = packedchar[i].x1 / (float) width;
        g.t1 = packedchar[i].y1 / (float) height;

        glyphs.emplace(unique_codepoints[i], g);
    }

    return FontAtlas(this, atlas, glyphs, width, height);
}

int Font::getKerning(const Glyph& left, const Glyph& right) const
{
    return stbtt_GetGlyphKernAdvance(&m_info, left.index, right.index);
}

void Font::getVerticalMetrics(int* ascent, int* descent, int* linegap) const { *ascent = m_ascent; *descent = m_descent; *linegap = m_linegap; }