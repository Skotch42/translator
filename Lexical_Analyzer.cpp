#include "Lexical_Analyzer.h"
#include <array>
#include <iostream>

using T = Lexical_Analyzer;

const T::SemanticFunction T::SEMANTIC_FUNCTION[19] = 
{
    &T::f1, &T::f2, &T::f3, &T::f4, &T::f5, &T::f6, &T::f7, &T::f8,
    &T::f9, &T::f10, &T::f11, &T::f12, &T::f13, &T::f14, &T::f15, &T::f16,
    &T::f17, &T::f18, &T::f19
};

/*all possible symbols
    0 - char
    1 - num
    2 - < > \n \t \0
    3 - =
    4 - >
    5 - <
    6 - !
    7 - ch () [] {} / + - * ,
    8 - other
*/
const int SYMBOLS[] = 
{
    2, 8, 8, 8, 8, 8, 8, 8, 8, 2, 2, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    2, 6, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 8, 7,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 8, 7, 5, 3, 4, 8,
    8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 7, 8, 8,
    8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 7, 8, 8,
};

//all possible words
const std::array<std::string, 7> KEYWORDS = 
{
    "while", "if", "else", "read", "write", "int", "array",
};

const std::array<LEXEM_T, 7> KEYWORDS_LEXEMS = 
{
    LEXEM_T::WHILE_T, LEXEM_T::IF_T, LEXEM_T::ELSE_T, LEXEM_T::READ_T, LEXEM_T::WRITE_T, LEXEM_T::NEWINT_T, LEXEM_T::NEWARRAY_T
};

//semantic programs
const int SEMANTIC[7][9] = 
{
    {  1,  4,  4,  8, 11, 14, 17,  7, 19},
    {  2,  2,  3,  3,  3,  3,  3,  3, 19},
    { 19,  5,  6,  6,  6,  6,  6,  6, 19},
    {  9,  9,  9, 10,  9,  9,  9,  9, 19},
    { 13, 13, 13, 12, 19, 19, 19, 13, 19},
    { 16, 16, 16, 15, 19, 19, 19, 16, 19},
    { 19, 19, 19, 18, 19, 19, 19, 19, 19},
};

//shift or no shift
const int SHIFT_BACK[7][9] = 
{
    { 0,  0,  0,  0,  0,  0,  0,  0, 0},
    { 0,  0,  0,  1,  1,  1,  1,  1, 0},
    { 0,  0,  0,  1,  1,  1,  1,  1, 0},
    { 1,  1,  0,  0,  1,  1,  1,  1, 0},
    { 1,  1,  0,  0,  0,  0,  0,  1, 0},
    { 1,  1,  0,  0,  0,  0,  0,  1, 0},
    { 0,  0,  0,  0,  0,  0,  0,  0, 0},
};

/*state tranzitions
    S - 0
    I - 1
    C - 2
    A - 3
    T - 4
    D - 5
    F - 6
*/
const int TRANSITION[7][9] = 
{
    { 1, 2, 0, 3, 4, 5, 6, 0, 0 },
    { 1, 1, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 2, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0 },
};

Lexical_Analyzer::Lexical_Analyzer(std::string& text) : m_text(text) {}

Lexem Lexical_Analyzer::getLexem()
{
    int state = 0;
    bool finished = false;

    while (!finished && m_textPos <= m_text.size()) 
    {
        char ch = m_text[m_textPos];

        if(ch == '\n') 
        {
            ++m_row;
            m_column = 0;
        }
        else
        {
            ++m_column;
        }

        int symb = SYMBOLS[static_cast<size_t>(ch)];

        int program = SEMANTIC[state][symb];
        program--; // offset

        if (!SHIFT_BACK[state][symb])
        {
            m_textPos++;
        }

        state = TRANSITION[state][symb];

        finished = (this->*SEMANTIC_FUNCTION[program])(ch);
    }

    return Lexem(m_accumulatedString, m_lexemType, m_column, m_row);
}

bool Lexical_Analyzer::empty()
{
    return m_textPos >= m_text.size();
}

bool Lexical_Analyzer::f1(char ch)
{
    m_accumulatedString = ch;

    return false;
}

bool Lexical_Analyzer::f2(char ch)
{
    m_accumulatedString += ch;

    return false;
}

bool Lexical_Analyzer::f3(char)
{
    m_lexemType = LEXEM_T::NAME_T;

    for (size_t i = 0; i < KEYWORDS.size(); ++i)
    {
        if (m_accumulatedString == KEYWORDS[i])
        {
            m_lexemType = KEYWORDS_LEXEMS[i];
        }
    }

    return true;
}

bool Lexical_Analyzer::f4(char ch)
{
    m_accumulatedNumber = ch - '0';

    return false;
}

bool Lexical_Analyzer::f5(char ch)
{
    m_accumulatedNumber = m_accumulatedNumber * 10 + ch - '0';

    return false;
}

bool Lexical_Analyzer::f6(char)
{
    m_lexemType = LEXEM_T::CONST_INT_T;
    m_accumulatedString = std::to_string(m_accumulatedNumber);

    return true;
}

bool Lexical_Analyzer::f7(char ch)
{
    switch (ch) 
    {
    case '+':
        m_lexemType = LEXEM_T::PLUS_T;
        break;
    case '-':
        m_lexemType = LEXEM_T::MINUS_T;
        break;
    case '(':
        m_lexemType = LEXEM_T::L_PR_T;
        break;
    case ')':
        m_lexemType = LEXEM_T::R_PR_T;
        break;
    case '*':
        m_lexemType = LEXEM_T::MUL_T;
        break;
    case '/':
        m_lexemType = LEXEM_T::DIV_T;
        break;
    case '{':
        m_lexemType = LEXEM_T::L_BRACE_T;
        break;
    case '}':
        m_lexemType = LEXEM_T::R_BRACE_T;
        break;
    case '[':
        m_lexemType = LEXEM_T::L_SQUARE_T;
        break;
    case ']':
        m_lexemType = LEXEM_T::R_SQUARE_T;
        break;
    case ',':
        m_lexemType = LEXEM_T::COMMA_T;
        break;
    case '\0':
        m_lexemType = LEXEM_T::END_T;
        break;
    case ';':
        m_lexemType = LEXEM_T::END_T;
        break;
    default:
        m_lexemType = LEXEM_T::ERROR_T;
    }

    m_accumulatedString = ch;

    return true;
}

bool Lexical_Analyzer::f8(char ch)
{
    m_accumulatedString = ch;

    return false;
}

bool Lexical_Analyzer::f9(char ch)
{
    m_accumulatedString = ch;
    m_lexemType = LEXEM_T::ASSIGN_T;

    return true;
}

bool Lexical_Analyzer::f10(char ch)
{
    m_accumulatedString += ch;
    m_lexemType = LEXEM_T::EQUAL_T;

    return true;
}

bool Lexical_Analyzer::f11(char ch)
{
    m_accumulatedString = ch;

    return false;
}

bool Lexical_Analyzer::f12(char ch)
{
    m_accumulatedString += ch;
    m_lexemType = LEXEM_T::GOREQUAL_T;

    return true;
}

bool Lexical_Analyzer::f13(char)
{
    m_lexemType = LEXEM_T::GREATER_T;

    return true;
}

bool Lexical_Analyzer::f14(char ch)
{
    m_accumulatedString = ch;

    return false;
}

bool Lexical_Analyzer::f15(char ch)
{
    m_accumulatedString += ch;
    m_lexemType = LEXEM_T::LOREQUAL_T;

    return true;
}

bool Lexical_Analyzer::f16(char)
{
    m_lexemType = LEXEM_T::LESS_T;

    return true;
}

bool Lexical_Analyzer::f17(char ch)
{
    m_accumulatedString = ch;

    return false;
}

bool Lexical_Analyzer::f18(char ch)
{
    m_accumulatedString += ch;
    m_lexemType = LEXEM_T::NOTEQUAL_T;

    return true;
}

bool Lexical_Analyzer::f19(char)
{
    m_lexemType = LEXEM_T::ERROR_T;

    return true;
}