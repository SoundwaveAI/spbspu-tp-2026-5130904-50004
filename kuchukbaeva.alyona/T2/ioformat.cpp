#include "ioformat.hpp"
#include <iostream>
#include <iomanip>
#include <cctype>

kuchukbaeva::IOGuard::IOGuard(std::basic_ios<char>& s):
  s_(s),
  width_(s.width()),
  precision_(s.precision()),
  fmt_(s.flags()),
  fill_(s.fill())
{
}

kuchukbaeva::IOGuard::~IOGuard()
{
  s_.width(width_);
  s_.precision(precision_);
  s_.flags(fmt_);
  s_.fill(fill_);
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::DelimiterIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  char c = '0';
  in >> c;
  if (in && (c != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::LabelIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data = "";
  in >> data;
  if (in && (data != dest.exp)) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::KeyIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  std::string data = "";
  in >> data;
  if (data == "key1") {
    dest.ref = lab::Key::KEY1;
  } else if (data == "key2") {
    dest.ref = lab::Key::KEY2;
  } else if (data == "key3") {
    dest.ref = lab::Key::KEY3;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::DblLitIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  in >> dest.ref;
  char c = '0';
  in >> c;
  if (in && (std::tolower(c) != 'd')) {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::RatLspIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return in >> lab::DelimiterIO{'('} >> lab::DelimiterIO{':'}
            >> lab::LabelIO{"N"} >> dest.ref.first
            >> lab::DelimiterIO{':'} >> lab::LabelIO{"D"} >> dest.ref.second
            >> lab::DelimiterIO{':'} >> lab::DelimiterIO{')'};
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::StringIO&& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }
  return std::getline(in >> lab::DelimiterIO{'"'}, dest.ref, '"');
}
