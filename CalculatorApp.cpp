#include "CalculatorApp.hpp"

#include <cstring>

const unsigned int Nrows = 7;
const unsigned int Ncols = 5;
const float padding = 0.1f;

const int chars[] =
{
    L'=', 
    L'D', L'E', L'L',
    L'C', L'R',
    L'(', L')',

    L'+', L_MINUS[0], L_TIMES[0], L_DIVIDE[0], L'^',

    L'1', L'2', L'3',
    L'4', L'5', L'6',
    L'7', L'8', L'9',
    L'0', L'.',

    L's', L'i', L'n', 
    L'c', L'o', L's',
    L't', L'a',

    L'R', L'e',
    L'I', L'm',
    L'l',
    L_PI[0], L_SQRT[0],
    L'F', L'g', L' ', L'P', L'r',
    L'S', L'y', L'x',
    L'M', L'h', L'd'
};

GMath::Vec2f CalculatorApp::buttonSize(const unsigned int& width, const unsigned int& height) const
{
    return { width*m_buttonSize[0] - padding*m_buttonSize[0], height*m_buttonSize[1] - padding*m_buttonSize[1] };
}

GMath::Vec2f CalculatorApp::get_pos(const unsigned int& col, const unsigned int& row) const
{
    return { padding*m_buttonSize[0]/2.f + col*m_buttonSize[0], m_windowSize[1] - Nrows*m_buttonSize[1] + row*m_buttonSize[1] + padding*m_buttonSize[1]/2.f };
}

CalculatorApp::CalculatorApp()
    : m_window(270.f, 540.f, "Calculator"), m_windowSize(m_window.size()),
    m_font("Assets/Fonts/Lora/Lora-SemiBoldItalic.ttf"),
    m_buttonSize({ m_windowSize[0]/(float)Ncols, m_windowSize[0]/(float)Ncols }), // assume width < height
    m_Atlas(m_font.createAtlas(std::vector<int>(chars, chars + sizeof(chars) / sizeof(int)), m_buttonSize[0], 4)),
    m_buttonProgram({ Shader("./Shaders/Button/button.vert", GL_VERTEX_SHADER), Shader("./Shaders/Button/button.frag", GL_FRAGMENT_SHADER) }),
    m_stringProgram({ Shader("./Shaders/String/string.vert", GL_VERTEX_SHADER), Shader("./Shaders/String/string.frag", GL_FRAGMENT_SHADER) }),
    m_ButtonColourLoc(m_buttonProgram.getUniformLocation("buttonColour")),
    m_TextColourLoc(m_stringProgram.getUniformLocation("textColour")),
    m_projection(GMath::orthographicProjection(0.f, (float) m_windowSize[0], 0.f, (float) m_windowSize[1], -1.f, 1.f)),
    m_display(m_windowSize[0], m_windowSize[1] - Nrows * m_buttonSize[1], &m_Atlas),
    m_buttons
    (
        {
            { get_pos(0, 1), buttonSize(), &m_Atlas, L"CLR", {1.f, 0.f, 0.f} }, 
            { get_pos(1, 1), buttonSize(), &m_Atlas, L"DEL", {1.f, 0.f, 0.f} }, 
            { get_pos(2, 1), buttonSize(), &m_Atlas, L"=", {1.f, 0.f, 0.f} },

            { get_pos(0, 0), buttonSize(), &m_Atlas, L"+", {1.f, 1.f, 0.f} },
            { get_pos(1, 0), buttonSize(), &m_Atlas, L_MINUS, {1.f, 1.f, 0.f} },
            { get_pos(2, 0), buttonSize(), &m_Atlas, L_TIMES, {1.f, 1.f, 0.f} },
            { get_pos(3, 0), buttonSize(), &m_Atlas, L_DIVIDE, {1.f, 1.f, 0.f} },
            { get_pos(4, 0), buttonSize(), &m_Atlas, L"^", {1.f, 1.f, 0.f} },

            { get_pos(0, 2), buttonSize(), &m_Atlas, L"7" }, { get_pos(1, 2), buttonSize(), &m_Atlas, L"8" }, { get_pos(2, 2), buttonSize(), &m_Atlas, L"9" },
            { get_pos(0, 3), buttonSize(), &m_Atlas, L"4" }, { get_pos(1, 3), buttonSize(), &m_Atlas, L"5" }, { get_pos(2, 3), buttonSize(), &m_Atlas, L"6" },
            { get_pos(0, 4), buttonSize(), &m_Atlas, L"1" }, { get_pos(1, 4), buttonSize(), &m_Atlas, L"2" }, { get_pos(2, 4), buttonSize(), &m_Atlas, L"3" },
            { get_pos(0, 5), buttonSize(2, 1), &m_Atlas, L"0" }, { get_pos(2, 5), buttonSize(), &m_Atlas, L"." }, 
            { get_pos(0, 6), buttonSize(), &m_Atlas, L"e" }, { get_pos(1, 6), buttonSize(), &m_Atlas, L_PI }, { get_pos(2, 6), buttonSize(), &m_Atlas, L"i" },

            { get_pos(3, 1), buttonSize(), &m_Atlas, L"(", {0.f, 1.f, 1.f} }, { get_pos(4, 1), buttonSize(), &m_Atlas, L")", {0.f, 1.f, 1.f} },
            { get_pos(3, 2), buttonSize(), &m_Atlas, L"sin", {0.f, 1.f, 0.f} }, { get_pos(4, 2), buttonSize(), &m_Atlas, L"asin", {0.f, 1.f, 0.f} },
            { get_pos(3, 3), buttonSize(), &m_Atlas, L"cos", {0.f, 1.f, 0.f} }, { get_pos(4, 3), buttonSize(), &m_Atlas, L"acos", {0.f, 1.f, 0.f} },
            { get_pos(3, 4), buttonSize(), &m_Atlas, L"tan", {0.f, 1.f, 0.f} }, { get_pos(4, 4), buttonSize(), &m_Atlas, L"atan", {0.f, 1.f, 0.f} },
            { get_pos(3, 5), buttonSize(), &m_Atlas, L"ln", {0.f, 1.f, 0.f} }, { get_pos(4, 5), buttonSize(), &m_Atlas, L_SQRT, {0.f, 1.f, 0.f} },
            { get_pos(3, 6), buttonSize(), &m_Atlas, L"Re", {0.f, 1.f, 0.f} }, { get_pos(4, 6), buttonSize(), &m_Atlas, L"Im", {0.f, 1.f, 0.f} }
        }
    )
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

void CalculatorApp::update()
{
    Event e;
    m_window.pollEvents();

    while (m_window.hasEvent(e))
    {
        switch (e.type)
        {
            case EventType::KEY:
                if (e.key.code == GLFW_KEY_ESCAPE)
                    m_window.close();
                break;
            case EventType::MOUSE:
                for (auto& i : m_buttons)
                {
                    if (i.isPressed(e.mouse))
                    {
                        if (i.getString() == L"=")
                        {
                            try 
                            {
                                std::vector<Calculator::Token> tokens = m_display.getTokens();
                                rpncalc.expression = tokens;
                                if (rpncalc.expression.size() != 0)
                                    m_display.setOutput(rpncalc.calculate());
                            }
                            catch (const std::exception& e)
                            {
                                m_display.setError(e.what());
                            }
                        }
                        else if (i.getString() == L"DEL") m_display.deleteToken();
                        else if (i.getString() == L"CLR") m_display.clearTokens();
                        else m_display.push(i.getString());

                    }
                }
                break;
            default:
                break;
        }
    }
}

void CalculatorApp::render() const
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_buttonProgram.use();
    glUniformMatrix4fv(m_buttonProgram.getUniformLocation("projection"), 1, GL_FALSE, m_projection.valuePtr());
    for (auto& b : m_buttons) b.renderButton(m_ButtonColourLoc);

    m_stringProgram.use();
    glUniformMatrix4fv(m_stringProgram.getUniformLocation("projection"), 1, GL_FALSE, m_projection.valuePtr());
    for (auto& b : m_buttons) b.renderText(m_TextColourLoc);
    m_display.render(m_TextColourLoc);

    m_window.swapBuffers();
}

void CalculatorApp::run() { while (m_window.isOpen()) { update(); render(); } }