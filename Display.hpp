#pragma once

#include "Utilities/String.hpp"
#include "Utilities/Rect.hpp"
#include "RPNCalculator.hpp"

#define L_PI L"\u03C0"
#define L_SQRT L"\u221A"
#define L_TIMES L"\u00D7"
#define L_DIVIDE L"\u00F7"
#define L_MINUS L"\u2212"

class Display
{
    Texture m_inputTexture, m_outputTexture;

    int m_width, m_height;
    std::vector<std::wstring> m_tokens;

    String m_inputString, m_outputString;

    std::wstring formatNumber(const long double& number) const;
    bool isNumber(const std::wstring& str) const;
    void update();

public:
    Display(const int& width, const int& height, const FontAtlas* pAtlas);

    void push(const std::wstring& string);
    void deleteToken();
    void clearTokens();
    std::vector<Calculator::Token> getTokens();
    void setOutput(const Calculator::Number& value);
    void setError(const std::string& error);
    void render(const GLuint& textColourLoc) const;
};