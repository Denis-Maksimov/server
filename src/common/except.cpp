#include "configure/version.h"
#ifdef  _VERSION_0_1
#include "common/except.hpp"


namespace common
{

m_exception::m_exception(std::string s) _GLIBCXX_USE_NOEXCEPT
{
    this->str=s;
}

const char*
m_exception::what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT
{
    return this->str.c_str();
}

}













#endif
