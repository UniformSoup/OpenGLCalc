#pragma once

#include <cmath>
#include <cfenv>
#include <vector>
#include <stack>
#include <stdexcept>
#include <complex>

namespace Calculator
{
    using Number = std::complex<long double>;
    enum class Type { NUMBER, OPERATOR, FUNCTION, PARENTHESIS};
    enum Operator { SUBTRACT = 0, ADD, MULTIPLY, DIVIDE, EXPONENTIATE }; // ordered by precedence. modulo not added beacuse complex modulo hard to define lol
    enum class Function { SQRT, LOG, ASIN, ACOS, ATAN, SIN, COS, TAN, ABS, NEGATE, REAL, IMAG }; // Same goes for floor and ceil
    enum class Parenthesis { LEFT, RIGHT };

    struct Token
    {
        Type type;
        
        union 
        {
            Number number;
            Operator op;
            Function func;
            Parenthesis paren;
        };

        Token(const Number& n);
        Token(const Operator& o);
        Token(const Function& f);
        Token(const Parenthesis& p);
    };

    class RPNCalculator
    {
        Number evaluateOperator(std::stack<Number>& numbers, const Token& t) const;
        Number evaluateFunction(std::stack<Number>& numbers, const Token& t) const;
        Number evaluateRPN(const std::vector<Token>& rpn) const;
        std::vector<Token> evaluateExpression() const;

    public:
        std::vector<Token> expression;
        Number calculate() const;

    };

    class SyntaxError : public std::exception
    {
    public:
        const char* what() const noexcept override { return "Syntax Error"; }
    };

    class InvalidToken : public Token, public std::exception 
    {
    public:
        InvalidToken(const Token& t) : Token(t) {};
        const char* what() const noexcept override { return "Invalid Token"; }; 
    };

    class MismatchedParenthesis : public std::exception
    {
    public:
        Parenthesis missing;
        MismatchedParenthesis(const Parenthesis& p) : missing(p) {}; 
        const char* what() const noexcept override { return "Mismatched Parenthesis"; }; 
    };

    class FloatingPointException : public std::exception
    {
    public:
        fexcept_t FENVexception;
        Token token;

        FloatingPointException(const fexcept_t& except, const Token& t) noexcept
            : FENVexception(except), token(t) {};

        const char* what() const noexcept override { return "Floating Point Error"; }
    };
}