#ifndef IOFORMAT_HPP
#define IOFORMAT_HPP

#include <ios>
#include <istream>
#include <ostream>
#include <string>
#include <utility>

namespace kuchukbaeva
{
  enum class Key
  {
    KEY1,
    KEY2,
    KEY3
  };

  struct DelimiterIO
  {
    char exp;
  };

  struct LabelIO
  {
    std::string exp;
  };

  struct KeyIO
  {
    Key& ref;
  };

  struct DblLitIO
  {
    double& ref;
  };

  struct RatLspIO
  {
    std::pair< long long, unsigned long long >& ref;
  };

  struct StringIO
  {
    std::string& ref;
  };

  struct DblLitOut
  {
    const double& ref;
  };

  struct RatLspOut
  {
    const std::pair< long long, unsigned long long >& ref;
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
  std::istream& operator>>(std::istream& in, LabelIO&& dest);
  std::istream& operator>>(std::istream& in, KeyIO&& dest);
  std::istream& operator>>(std::istream& in, DblLitIO&& dest);
  std::istream& operator>>(std::istream& in, RatLspIO&& dest);
  std::istream& operator>>(std::istream& in, StringIO&& dest);

  std::ostream& operator<<(std::ostream& out, const DblLitOut& src);
  std::ostream& operator<<(std::ostream& out, const RatLspOut& src);
}

#endif
