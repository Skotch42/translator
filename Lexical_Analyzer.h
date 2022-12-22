#pragma once

#include <string>
#include <vector>
#include "Lexem.h"

class Lexical_Analyzer
{
public:

    using SemanticFunction = bool(Lexical_Analyzer::*)(char);
    Lexical_Analyzer(std::string &text);
    Lexem getLexem();
    bool empty();

private:
    static const SemanticFunction SEMANTIC_FUNCTION[19];

    std::string m_text;
    uint32_t m_textPos = 0;
    uint32_t m_row = 0;
    uint32_t m_column = 0;
    std::string m_accumulatedString = "";
    int64_t m_accumulatedNumber = 0;
    LEXEM_T m_lexemType;

    bool f1(char), f2(char), f3(char), f4(char), f5(char),
         f6(char), f7(char), f8(char), f9(char), f10(char),
        f11(char), f12(char), f13(char), f14(char), f15(char),
        f16(char), f17(char), f18(char), f19(char);
};