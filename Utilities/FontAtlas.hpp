#pragma once

#include <map>
#include "Glyph.hpp"
#include "Resource.hpp"

#include <stdexcept>

class Font;

struct CharacterNotGenerated : std::exception
{
  const char* what() const noexcept { return "Character Not Generated."; }
};

class FontAtlas
{
    const Font* m_pFont;
    Texture m_atlas;
    std::map<int, Glyph> m_glyphs;
    int m_width, m_height;

public:
    FontAtlas(const Font* pFont, const Texture& atlas, const std::map<int, Glyph>& glyphs, const int& width, const int& height);
    const Font * getFont() const { return m_pFont; }
    void bind() const { m_atlas->bind(); }
    Glyph operator[](const wchar_t& c) const
    {
        if (m_glyphs.find(c) == m_glyphs.end())
            throw CharacterNotGenerated();

        return m_glyphs.at(c);
    }
};