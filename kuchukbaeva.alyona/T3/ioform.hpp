#ifndef IOFORM_HPP
#define IOFORM_HPP

#include <ios>
#include <istream>

namespace kuchukbaeva
{
  struct DelimiterIO
  {
    char exp;
  };

  class IOGuard
  {
  public:
    explicit IOGuard(std::basic_ios< char >& s);
    ~IOGuard();
  private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
    char fill_;
  };

  std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
}

#endif
