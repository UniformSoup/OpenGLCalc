#include "FontAtlas.hpp"

FontAtlas::FontAtlas(const Font* pFont, const Texture& atlas, const std::map<int, Glyph>& glyphs, const int& width, const int& height)
    : m_pFont(pFont), m_atlas(atlas), m_glyphs(glyphs), m_width(width), m_height(height) {}