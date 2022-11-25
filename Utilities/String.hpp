#pragma once

#include "Font.hpp"
#include "Glyph.hpp"
#include "Resource.hpp"
#include "GraphicsMath.hpp"

class String
{
public:
    enum class Alignment { Left, Center, Right };
    String( const FontAtlas* atlas, const std::wstring& string, const GMath::Vec2f& pos, 
            const GMath::Vec2f& size, const GMath::Vec4f& colour = { 0, 0, 0, 1 }, const float& xpadding = 0.1f, const Alignment& type = Alignment::Center);
    std::wstring getString() const;
    void setString(const std::wstring& string);
    void render(const GLuint& textColourLoc) const;
private:
    VertexArray m_VAO;
    Buffer<1> m_VBO;

    GMath::Vec4f m_colour; 
    std::wstring m_string;
    const FontAtlas* m_patlas;
    std::vector<std::vector<Glyph>> m_glyphs;

    GMath::Vec2f m_pos, m_size;
    float m_scale;
    float m_xpadding;
    std::vector<float> m_widths;
    float m_maxWidth;
    Alignment m_type;

    void update();
    void updateGlyphs();
    void updateWidths();
};