#include "Lexem.h"

Lexem::Lexem(std::string word, LEXEM_T type, uint32_t col, uint32_t row) : m_type(type), m_word(word), col(col), row(row) {}

std::string Lexem::word() const 
{
    return this->m_word;
}

LEXEM_T Lexem::type() const 
{
    return this->m_type;
}