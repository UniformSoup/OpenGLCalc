#include "String.hpp"

#include <glad/glad.h>

#include <cmath>

String::String( const FontAtlas* patlas, const std::wstring& string, const GMath::Vec2f& pos, 
        const GMath::Vec2f& size, const GMath::Vec4f& colour, const float& xpadding, const Alignment& type)
        : m_colour(colour), m_string(string), m_patlas(patlas), m_pos(pos), m_size(size), m_scale(1.f), m_xpadding(xpadding), m_type(type)
{
    m_VAO->bind();
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO->ID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*) (2 * sizeof(float))); // offset 2 floats
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    update();
}

std::wstring String::getString() const { return m_string; }
void String::setString(const std::wstring& string) { m_string = string; update(); }
void String::render(const GLuint& textColourLoc) const
{
    glUniform4fv(textColourLoc, 1, m_colour.data());
    int ascent, descent, linegap;
    m_patlas->getFont()->getVerticalMetrics(&ascent, &descent, &linegap);

    m_patlas->bind();

    for (size_t j = 0; j < m_glyphs.size(); ++j)
    {
        float x = 0, y = 0;
        switch(m_type)
        {
            case Alignment::Left:
                x = m_pos[0] + m_xpadding * m_size[0];
                y = m_pos[1] + m_size[1] - (ascent - descent + linegap) * m_scale * (m_glyphs.size() - j) + linegap * m_scale;
                break;
            case Alignment::Center:
                x = m_pos[0] + (m_size[0] - m_widths[j] * m_scale) / 2.f;
                y = m_pos[1] + (m_size[1] - (ascent - descent + linegap) * m_scale * m_glyphs.size() + linegap * m_scale) / 2.f + (ascent - descent + linegap) * m_scale * j - linegap * m_scale;
                break;
            case Alignment::Right:
                x = m_size[0] - m_widths[j] * m_scale - m_xpadding * m_size[0];
                y = m_pos[1] + m_size[1] - (ascent - descent + linegap) * m_scale * (m_glyphs.size() - j) + linegap * m_scale;
                break;
        }

        for(size_t i = 0; i < m_glyphs[j].size(); ++i)
        {
            float w = (m_glyphs[j][i].x1 - m_glyphs[j][i].x0) * m_scale;
            float h = (m_glyphs[j][i].y1 - m_glyphs[j][i].y0) * m_scale;
            float xoffset = m_glyphs[j][i].x0 * m_scale;
            float yoffset = ( - m_glyphs[j][i].y1 + ascent) * m_scale;

            float chardata[6][4] =
            {
                { x + xoffset,     y + yoffset + h,    m_glyphs[j][i].s0, m_glyphs[j][i].t1 },            
                { x + xoffset,     y + yoffset,        m_glyphs[j][i].s0, m_glyphs[j][i].t0 },
                { x + xoffset + w, y + yoffset,        m_glyphs[j][i].s1, m_glyphs[j][i].t0 },

                { x + xoffset,     y + yoffset + h,    m_glyphs[j][i].s0, m_glyphs[j][i].t1 },
                { x + xoffset + w, y + yoffset,        m_glyphs[j][i].s1, m_glyphs[j][i].t0 },
                { x + xoffset + w, y + yoffset + h,    m_glyphs[j][i].s1, m_glyphs[j][i].t1 }           
            };

            glBindBuffer(GL_ARRAY_BUFFER, m_VBO->ID[0]);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(chardata), chardata); 
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            m_VAO->bind();
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += m_glyphs[j][i].advanceWidth * m_scale;
        }
    }
}

void String::update()
{
    updateGlyphs();
    updateWidths();

    int ascent, descent, linegap;
    m_patlas->getFont()->getVerticalMetrics(&ascent, &descent, &linegap);

    float xScale = m_size[0] * (1 - 2.f*m_xpadding) / m_maxWidth;
    float yScale = m_size[1] / (float) (m_glyphs.size() * (ascent - descent + linegap) - linegap);

    m_scale = std::min(xScale, yScale);
}

void String::updateGlyphs()
{
    m_glyphs = { {} };

    for (auto& c : m_string)
    {
        if (c == '\n')
            m_glyphs.emplace_back();
        else
            m_glyphs.back().emplace_back((*m_patlas)[c]);
    }
}

void String::updateWidths()
{
    m_widths = {};
    m_maxWidth = 0;

    for (auto& line : m_glyphs)
    {
        float lineWidth = 0;

        for (size_t i = 0; i < line.size(); ++i)
        {
            lineWidth += line[i].advanceWidth;
            if (i + 1 < line.size())
                lineWidth += m_patlas->getFont()->getKerning(line[i], line[i + 1]);
        }

        m_maxWidth = std::max(m_maxWidth, lineWidth);
        m_widths.emplace_back(lineWidth);
    }
}