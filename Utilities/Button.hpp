#pragma once

#include "Program.hpp"
#include "String.hpp"
#include "Rect.hpp"
#include "Window.hpp"

#include <cmath>

// Make a grid button class to simplify all the code away

class Button : protected Rect
{   
    String string;
    const GMath::Vec3f m_colour;
    mutable GMath::Vec3f m_currentColour;

public:
    Button(const GMath::Vec2f& pos, const GMath::Vec2f& size, const FontAtlas* pAtlas, const std::wstring& str, const GMath::Vec3f& colour = {1.f, 1.f, 1.f})
            : Rect(pos, size), string(pAtlas, str, pos, size),
            m_colour(colour), m_currentColour(m_colour) {}

    void renderButton(const GLuint& buttonColourLoc) const
    {
        VAO->bind();
        glUniform3fv(buttonColourLoc, 1, m_currentColour.valuePtr());
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    }

    void renderText(const GLuint& textColourLoc) const
    {
        string.render(textColourLoc);
    }

    bool isPressed(const Mouse& mouseEvent) const
    {
        if (mouseEvent.button == GLFW_MOUSE_BUTTON_1)
        {
            if (mouseEvent.action == GLFW_PRESS && 
                GMath::contains(m_pos, m_size, {(float) mouseEvent.pos[0], (float) mouseEvent.pos[1]}))
            {
                m_currentColour = m_colour * 0.6f; 
                return true;
            }
        }
        if (mouseEvent.action == GLFW_RELEASE) m_currentColour = m_colour;
        return false;
    }

    std::wstring getString() const { return string.getString(); }
};