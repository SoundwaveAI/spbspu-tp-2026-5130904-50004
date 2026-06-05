#include "ioform.hpp"
#include <iostream>

kuchukbaeva::IOGuard::IOGuard(std::basic_ios< char >& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{}

kuchukbaeva::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(fmt_);
  s_.fill(fill_);
}

std::istream& kuchukbaeva::operator>>(std::istream& in, DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
