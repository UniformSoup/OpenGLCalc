#include "Display.hpp"

#include "RPNCalculator.hpp"

std::vector<Calculator::Token> Display::getTokens()
{
    try
    {
        using namespace Calculator;
        std::vector<Token> res;
        for (size_t i = 0; i < m_tokens.size(); ++i)
        {
            if (m_tokens[i] == L_MINUS)
            {
                if (res.size() != 0 && res.back().type == Type::NUMBER) res.emplace_back(Operator::SUBTRACT);
                else                                                    res.emplace_back(Function::NEGATE);
            }
            else if (m_tokens[i] == L"+")        res.emplace_back(Operator::ADD);
            else if (m_tokens[i] == L_TIMES)     res.emplace_back(Operator::MULTIPLY);
            else if (m_tokens[i] == L_DIVIDE)    res.emplace_back(Operator::DIVIDE);
            else if (m_tokens[i] == L"^")        res.emplace_back(Operator::EXPONENTIATE);
            else if (m_tokens[i] == L"(")        res.emplace_back(Parenthesis::LEFT);
            else if (m_tokens[i] == L")")        res.emplace_back(Parenthesis::RIGHT);
            else if (m_tokens[i] == L_SQRT)      res.emplace_back(Function::SQRT);
            else if (m_tokens[i] == L"ln")       res.emplace_back(Function::LOG);
            else if (m_tokens[i] == L"sin")      res.emplace_back(Function::SIN);
            else if (m_tokens[i] == L"cos")      res.emplace_back(Function::COS);
            else if (m_tokens[i] == L"tan")      res.emplace_back(Function::TAN);
            else if (m_tokens[i] == L"asin")     res.emplace_back(Function::ASIN);
            else if (m_tokens[i] == L"acos")     res.emplace_back(Function::ACOS);
            else if (m_tokens[i] == L"atan")     res.emplace_back(Function::ATAN);
            else if (m_tokens[i] == L"Re")       res.emplace_back(Function::REAL);
            else if (m_tokens[i] == L"Im")       res.emplace_back(Function::IMAG);
            else if (m_tokens[i] == L"e")        res.emplace_back(2.71828182845904523536L);
            else if (m_tokens[i] == L_PI)        res.emplace_back(3.14159265358979323846L);
            else if (m_tokens[i] == L"i")        res.emplace_back(Number(0, 1));
            else                                 res.emplace_back(std::stod(m_tokens[i]));
        }
        return res;
    }
    catch (...)
    {
        throw Calculator::SyntaxError();
    }
}

Display::Display(const int& width, const int& height, const FontAtlas* pAtlas)
    : m_width(width), m_height(height), m_inputString(pAtlas , L"", { 0.f, 0.f }, { (float) width, height / 2.f }, { 1, 1, 1, 1 }),
    m_outputString(pAtlas , L"", { 0.f, height / 2.f }, { (float) width, height / 2.f }, { 1, 1, 1, 1 })
{
} 

std::wstring Display::formatNumber(const long double& number) const
{
    std::wstring res = std::to_wstring(number);
    // remove trailing zeroes and decimal points.
    res = res.substr(0, res.find_last_not_of(L"0") + 1);
    // if no characters left it must be 0
    if (res.back() == L'.') res = res.substr(0, res.size() - 1);
    return res;
}

void Display::push(const std::wstring& string)
{
    if (m_tokens.size() != 0 && isNumber(m_tokens.back()) && isNumber(string))
        m_tokens.back() += string;
    else 
        m_tokens.emplace_back(string);
        
    m_outputString.setString(L"");
    update();
}

void Display::deleteToken()
{
    if (m_tokens.size() != 0)
    {
        if (isNumber(m_tokens.back()))
        {
            const std::wstring backstr = m_tokens.back();
            m_tokens.back() = backstr.substr(0, backstr.size() - 1);
            if (m_tokens.back() == L"") m_tokens.pop_back();
        }
        else
            m_tokens.pop_back();

        m_outputString.setString(L"");
        update();
    }
}

void Display::clearTokens()
{
    m_tokens.clear();
    m_outputString.setString(L"");
    update();
}

// only checks the first character in the string.
// assumes the string is not empty.
bool Display::isNumber(const std::wstring& str) const
{
    return str[0] == L'.' || std::isdigit(str[0]);
}

void Display::update()
{
    std::wstring input;
    for(const auto& t : m_tokens) input += t;
    m_inputString.setString(input);
}

void Display::setOutput(const Calculator::Number& value)
{
    std::wstring real = formatNumber(std::abs((value.real()))), imag = formatNumber(std::abs((value.imag())));
    if (imag == L"1") imag.clear();

    if (real == L"0" && imag == L"0") 
        m_outputString.setString(L"0");
    else if (real != L"0" && imag == L"0")
        m_outputString.setString(((value.real() < 0) ? L"− " : L"") + real);
    else if (real == L"0" && imag != L"0")
        m_outputString.setString(((value.imag() < 0) ? L"− " : L"") + imag + L"i");
    else
        m_outputString.setString(((value.real() < 0) ? L"− " : L"") + real + ((value.imag() < 0) ? L" − " : L" + ") + imag + L"i");

    update();
}

void Display::setError(const std::string& error)
{
    m_outputString.setString({error.begin(), error.end()});
    update();
}

void Display::render(const GLuint& textColourLoc) const
{
    m_inputString.render(textColourLoc);
    m_outputString.render(textColourLoc);
}