#include "RPNCalculator.hpp"

//#pragma STDC FENV_ACCESS ON

namespace Calculator
{
    Token::Token(const Number& n) : type(Type::NUMBER), number(n) {}
    Token::Token(const Operator& o): type(Type::OPERATOR), op(o) {}
    Token::Token(const Function& f) : type(Type::FUNCTION), func(f) {}
    Token::Token(const Parenthesis& p) : type(Type::PARENTHESIS), paren(p) {}

    Number RPNCalculator::calculate() const { return evaluateRPN(evaluateExpression()); }

    Number RPNCalculator::evaluateRPN(const std::vector<Token>& rpn) const
    {
        std::stack<Number> numbers;

        std::feclearexcept(FE_ALL_EXCEPT);

        for (const Token& t : rpn)
        {
            switch (t.type)
            {
                case Type::NUMBER:
                    numbers.push(t.number);
                    break;

                case Type::OPERATOR:
                    if (numbers.size() < 2) throw SyntaxError();
                    numbers.push(evaluateOperator(numbers, t));
                    break;
                case Type::FUNCTION:
                    if (numbers.empty()) throw SyntaxError();
                    numbers.push(evaluateFunction(numbers, t));
                    break;
                default:
                    throw SyntaxError();
            }

            fexcept_t result = std::fetestexcept(FE_ALL_EXCEPT & ~FE_INEXACT);
            if (result) throw FloatingPointException(result, t);
        }

        if (numbers.size() != 1) throw SyntaxError();

        return numbers.top();
    }

    /* See the Shunting Yard Algorithm https://en.wikipedia.org/wiki/Shunting_yard_algorithm */
    std::vector<Token> RPNCalculator::evaluateExpression() const
    {
        std::vector<Token> output; 
        std::stack<Token> operators; 

        output.reserve(expression.size());

        for (const Token& t : expression)
        {
            switch (t.type)
            {
                case Type::NUMBER:      output.push_back(t); break;
                case Type::FUNCTION:    operators.push(t); break;
                case Type::OPERATOR:
                    while(!operators.empty())
                    {
                        // While the stack holds a function or an operator of lower precedence.
                        if  (
                            (operators.top().type == Type::OPERATOR && operators.top().op > t.op)
                            || operators.top().type == Type::FUNCTION
                            )
                        {
                            output.push_back(operators.top()); operators.pop();
                        } 
                        else
                            break;
                    }
                    operators.push(t);
                    break;
                case Type::PARENTHESIS:
                    if (t.paren == Parenthesis::RIGHT)
                    {
                        while (!operators.empty() && operators.top().type != Type::PARENTHESIS)
                        {
                            output.push_back(operators.top()); operators.pop();
                        }

                        if (!operators.empty()) operators.pop();
                        else throw MismatchedParenthesis(Parenthesis::LEFT);

                        if (!operators.empty() && operators.top().type == Type::FUNCTION)
                        {
                            output.push_back(operators.top()); operators.pop();
                        } 
                    }
                    else operators.push(t);
                    break;
            }
        }

        for (; !operators.empty(); operators.pop())
        {
            if (!operators.empty() && operators.top().type == Type::PARENTHESIS) throw MismatchedParenthesis(Parenthesis::RIGHT);
            output.push_back(operators.top());
        }

        return output;
    }

    Number RPNCalculator::evaluateOperator(std::stack<Number>& numbers, const Token& t) const
    {

        Number right = numbers.top(); numbers.pop();
        Number left = numbers.top(); numbers.pop();
        switch (t.op)
        {
            case Operator::SUBTRACT:        return left - right;
            case Operator::ADD:             return left + right;
            case Operator::MULTIPLY:        return left * right;
            case Operator::DIVIDE:          return left / right;
            case Operator::EXPONENTIATE:    return std::pow(left, right);
            default:                        throw InvalidToken(t);
        }
    }

    Number RPNCalculator::evaluateFunction(std::stack<Number>& numbers, const Token& t) const
    {
        Number right = numbers.top(); numbers.pop();
        switch (t.func)
        {
            case Function::SQRT:    return std::sqrt(right);
            case Function::LOG:     return std::log(right);
            case Function::SIN:     return std::sin(right);
            case Function::COS:     return std::cos(right);
            case Function::TAN:     return std::tan(right);
            case Function::ASIN:    return std::asin(right);
            case Function::ACOS:    return std::acos(right);
            case Function::ATAN:    return std::atan(right);
            case Function::NEGATE:  return -right;
            case Function::ABS:     return std::abs(right);
            case Function::REAL:    return std::real(right);
            case Function::IMAG:    return std::imag(right);
            default:                throw InvalidToken(t);
        }
    }
}