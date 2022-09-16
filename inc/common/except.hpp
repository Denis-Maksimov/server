#ifndef EXCEPT
#define EXCEPT
#include "configure/version.h"
#ifdef  _VERSION_0_1
#include <string>
#include <exception>

namespace common
{

class m_exception : public std::exception
  {
  public:
    m_exception() _GLIBCXX_USE_NOEXCEPT { }
    m_exception(std::string s) _GLIBCXX_USE_NOEXCEPT;
    virtual ~m_exception() _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;

    /** Returns a C-style character string describing the general cause
     *  of the current error.  */
    virtual const char*
    what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_USE_NOEXCEPT;
    private:
    std::string str;

  };



}







#endif //_VERSION_0_1
#endif /* EXCEPT */
