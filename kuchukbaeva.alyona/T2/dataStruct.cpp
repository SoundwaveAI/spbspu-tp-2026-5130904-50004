#include "dataStruct.hpp"
#include "ioformat.hpp"
#include <limits>
#include <iostream>
#include <iomanip>

namespace kuchukbaeva {
  struct KeyValueIO {
    DataStruct& dest;
    int& mask;
  };
  std::istream& operator>>(std::istream& in, KeyValueIO&& dest);
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::KeyValueIO&& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  kuchukbaeva::Key key = kuchukbaeva::Key::KEY1;
  in >> kuchukbaeva::DelimiterIO{':'} >> kuchukbaeva::KeyIO{key};

  if (key == kuchukbaeva::Key::KEY1 && !(dest.mask & 1)) {
    in >> kuchukbaeva::DblLitIO{dest.dest.key1};
    dest.mask |= 1;
  } else if (key == kuchukbaeva::Key::KEY2 && !(dest.mask & 2)) {
    in >> kuchukbaeva::RatLspIO{dest.dest.key2};
    dest.mask |= 2;
  } else if (key == kuchukbaeva::Key::KEY3) {
    in >> kuchukbaeva::StringIO{dest.dest.key3};
    dest.mask |= 4;
  } else {
    in.setstate(std::ios::failbit);
  }
  return in;
}

bool kuchukbaeva::operator<(const kuchukbaeva::DataStruct& lhs, const kuchukbaeva::DataStruct& rhs) {
  if (lhs.key1 != rhs.key1) {
    return lhs.key1 < rhs.key1;
  }

  const double rat1 = static_cast< double >(lhs.key2.first) / lhs.key2.second;
  const double rat2 = static_cast< double >(rhs.key2.first) / rhs.key2.second;
  if (rat1 != rat2) {
    return rat1 < rat2;
  }

  return lhs.key3.length() < rhs.key3.length();
}

std::istream& kuchukbaeva::operator>>(std::istream& in, kuchukbaeva::DataStruct& dest) {
  std::istream::sentry sentry(in);
  if (!sentry) {
    return in;
  }

  kuchukbaeva::DataStruct input = {0.0, {0, 0}, ""};
  int mask = 0;
  in >> kuchukbaeva::DelimiterIO{'('}
     >> kuchukbaeva::KeyValueIO{input, mask}
     >> kuchukbaeva::KeyValueIO{input, mask}
     >> kuchukbaeva::KeyValueIO{input, mask}
     >> kuchukbaeva::DelimiterIO{':'} >> kuchukbaeva::DelimiterIO{')'};

  if (in) {
    dest = input;
  } else {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    in >> dest;
  }
  return in;
}

std::ostream& kuchukbaeva::operator<<(std::ostream& out, const kuchukbaeva::DataStruct& src) {
  std::ostream::sentry sentry(out);
  if (!sentry) {
    return out;
  }
  kuchukbaeva::IOGuard fmtguard(out);
  out << std::fixed << std::setprecision(1);
  out << "(:key1 " << kuchukbaeva::DblLitOut{src.key1};
  out  << ":key2 " << kuchukbaeva::RatLspOut{src.key2};
  out  << ":key3 \"" << src.key3 << "\":)";
  return out;
}
