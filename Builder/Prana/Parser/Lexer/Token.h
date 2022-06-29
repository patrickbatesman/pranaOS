#pragma once

#include <memory>
#include <string>
#include <utility>

class Parser;
class Lexer;

class Token {
    friend class Parser;
    friend class Lexer;

public:
    enum class Type {
        Default,
        SubRule,
        Comma,
        Equal,
        VariableBegin,
        VariableEnd,
        Variable,
    };  

public:
    Token(std::string content, Type type, int nesting, size_t line)
        : m_content(new std::string(std::move(content)))
        , m_type(type)
        , m_nesting(nesting)
        , m_line(line)
    {
    }

    Token(Type type, int nesting, size_t line)
        : m_type(type)
        , m_nesting(nesting)
        , m_line(line)
    {
    }

    static Token TokenFromChar(char c, int nesting, size_t line)
    {
        return Token(token_type(c), nesting, line);
    }


};