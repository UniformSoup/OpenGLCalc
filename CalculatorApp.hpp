#pragma once

#include "Utilities/Window.hpp"
#include "Utilities/Font.hpp"
#include "Utilities/FontAtlas.hpp"
#include "Utilities/Program.hpp"

#include "Utilities/Button.hpp"
#include "Display.hpp"

#include "RPNCalculator.hpp"

class CalculatorApp
{
    Calculator::RPNCalculator rpncalc;
    Window m_window;
    GMath::Vec2i m_windowSize;
    Font m_font;
    GMath::Vec2f m_buttonSize;
    FontAtlas m_Atlas;
    Program m_buttonProgram, m_stringProgram;
    GLuint m_ButtonColourLoc, m_TextColourLoc;
    GMath::Mat4f m_projection;

    Display m_display;
    std::vector<Button> m_buttons;

    GMath::Vec2f buttonSize(const unsigned int& width = 1, const unsigned int& height = 1) const;
    GMath::Vec2f get_pos(const unsigned int& col, const unsigned int& row) const;

    void update();
    void render() const;

public:
    CalculatorApp();
    void run();

};