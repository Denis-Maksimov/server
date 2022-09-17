#include "configure/version.h"
#ifdef  _VERSION_0_1
#include "common/except.hpp"


// namespace common
// {
common::
m_exception::m_exception(std::string s) _GLIBCXX_USE_NOEXCEPT
{
    this->strn=s;
}

const char*
common::m_exception::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return this->strn.c_str();
}


common::
m_exception::~m_exception() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    
}











#endif
