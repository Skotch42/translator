#pragma once

#include <stack>
#include "RPN.h"
#include "Lexem.h"
#include "Lexical_Analyzer.h"

enum STATE 
{
    TERM = -1,
    P = 0,
    I = 1,
    M = 2,
    A = 3,
    N = 4,
    Q = 5,
    S = 6,
    H = 7,
    C = 8,
    D = 9,
    K = 10,
    E = 11,
    U = 12,
    T = 13,
    V = 14,
    F = 15,
    Z = 16,
};

enum STACK_ITEM_T 
{
    EMPTY_T,
    TERM_T,
    STATE_T
};

class StackItem : public Lexem 
{

    STATE state = STATE::TERM;
    STACK_ITEM_T stack_item_type;

public:

    StackItem(std::string word, LEXEM_T lexem_type) : Lexem(word, lexem_type), stack_item_type(STACK_ITEM_T::TERM_T) {}
    StackItem(std::string word) : Lexem(word, LEXEM_T::EMPTY_T), stack_item_type(STACK_ITEM_T::TERM_T) {}
    StackItem(LEXEM_T lexem_type) : Lexem("\0", lexem_type), stack_item_type(STACK_ITEM_T::TERM_T) {}
    StackItem() : Lexem("\0", LEXEM_T::EMPTY_T), stack_item_type(STACK_ITEM_T::EMPTY_T) {}
    StackItem(LEXEM_T lexem_type, STACK_ITEM_T stack_item_type) : Lexem("\0", lexem_type), stack_item_type(stack_item_type) {}
    StackItem(STATE state) : state(state), stack_item_type(STACK_ITEM_T::STATE_T) {}

    STACK_ITEM_T get_stack_item_type() const 
    { 
        return stack_item_type;
    }

    STATE get_state() const 
    { 
        return state; 
    }
};

class Parser 
{

    typedef void (Parser::* functionalArray)();
    static const functionalArray funcArr[47];
    static const int M[17][28];
    bool is_filling_vars = false;
    bool is_filling_arr = false;
    Lexem lexem;
    std::string lexem_word;
    LEXEM_T lexem_type;
    int k = 0;
    std::stack<StackItem> stack1, stack2;
    std::stack<int> marks;
    Lexical_Analyzer& m_LexicalA;
    RPN cur_RPN;

    Lexem getLexem();
    void step(STACK_ITEM_T, STACK_ITEM_T);
    void reset();
    bool isSemantic(std::stack<StackItem> &stack2);
    void executeSemantic(std::stack<StackItem> &stack2);
    void count();

    void f1(), f2(), f3(), f4(), f5(), f6(), f7(), f8(), f9(), f10(), f11(), f12(), f13(), f14(), f15(),
            f16(), f17(), f18(), f19(), f20(), f21(), f22(), f23(), f24(), f25(), f26(), f27(), f28(), f29(), f30(),
            f31(), f32(), f33(), f34(), f35(), f36(), f37(), f38(), f39(), f40(), f41(), f42(), f43(), f44(), f45(), f46(), f47();

public:

    Parser(Lexical_Analyzer &l);
    RPN make_RPN();
};
