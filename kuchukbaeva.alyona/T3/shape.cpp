#include "shape.hpp"
#include "ioform.hpp"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <numeric>
#include <cmath>

namespace
{
  bool isRightAngle(const kuchukbaeva::Point& p1, const kuchukbaeva::Point& p2, const kuchukbaeva::Point& p3)
  {
    long long dx1 = p1.x_ - p2.x_;
    long long dy1 = p1.y_ - p2.y_;
    long long dx2 = p3.x_ - p2.x_;
    long long dy2 = p3.y_ - p2.y_;
    return (dx1 * dx2 + dy1 * dy2) == 0;
  }

  struct RightAngleChecker
  {
    const kuchukbaeva::Polygon& poly_;
    size_t n_;

    RightAngleChecker(const kuchukbaeva::Polygon& p):
      poly_(p),
      n_(p.points_.size())
    {}

    bool operator()(size_t i) const
    {
      size_t prev = (i + n_ - 1) % n_;
      size_t next = (i + 1) % n_;
      return isRightAngle(poly_.points_[prev], poly_.points_[i], poly_.points_[next]);
    }
  };

  struct IotaGenerator
  {
    size_t current_ = 0;
    size_t operator()()
    {
      return current_++;
    }
  };

  struct TriangleAreaGenerator
  {
    const Polygon& poly_;
    size_t idx_;

    TriangleAreaGenerator(const Polygon& p):
      poly_(p),
      idx_(1)
    {}

    double operator()()
    {
      if (idx_ + 1 >= poly_.points_.size())
      {
        return 0.0;
      }
      double area = getTriangleArea(poly_.points_[0], poly_.points_[idx_], poly_.points_[idx_ + 1]);
      idx_++;
      return area;
    }
  };

  struct PointReader
  {
    std::istream& in_;
    kuchukbaeva::Point operator()()
    {
      kuchukbaeva::Point p{0, 0};
      if (in_)
      {
        in_ >> p;
      }
      return p;
    }
  };
}

bool kuchukbaeva::operator==(const Point& lhs, const Point& rhs)
{
  return (lhs.x_ == rhs.x_) && (lhs.y_ == rhs.y_);
}

std::istream& kuchukbaeva::operator>>(std::istream& in, Point& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  in >> DelimiterIO{'('} >> dest.x_ >> DelimiterIO{';'} >> dest.y_ >> DelimiterIO{')'};
  return in;
}

std::ostream& kuchukbaeva::operator<<(std::ostream& out, const Point& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << '(' << src.x_ << ';' << src.y_ << ')';
  return out;
}

bool kuchukbaeva::operator==(const Polygon& lhs, const Polygon& rhs)
{
  if (lhs.points_.size() != rhs.points_.size())
  {
    return false;
  }
  return std::equal(lhs.points_.begin(), lhs.points_.end(), rhs.points_.begin());
}

std::istream& kuchukbaeva::operator>>(std::istream& in, Polygon& dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }
  size_t count = 0;
  in >> count;
  if (!in || count < 3)
  {
    in.setstate(std::ios_base::failbit);
    return in;
  }
  std::vector< Point > temp;
  temp.reserve(count);
  std::generate_n(std::back_inserter(temp), count, PointReader{in});

  if (in)
  {
    dest.points_ = std::move(temp);
  }
  else
  {
    in.setstate(std::ios_base::failbit);
  }
  return in;
}

std::ostream& kuchukbaeva::operator<<(std::ostream& out, const Polygon& src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }
  out << src.points_.size();
  if (!src.points_.empty())
  {
    out << ' ' << src.points_.front();
    std::copy(std::next(src.points_.begin()), src.points_.end(), std::ostream_iterator< Point >(out, " "));
  }
  return out;
}

double kuchukbaeva::getArea(const Polygon& poly)
{
  double area = (p1.x_ - p0.x_) * (p2.y_ - p0.y_) - (p2.x_ - p0.x_) * (p1.y_ - p0.y_);
  return std::abs(area) / 2.0;
}

bool kuchukbaeva::hasRightAngle(const Polygon& poly)
{
  if (poly.points_.size() < 3)
  {
    return false;
  }
  std::vector< size_t > indices(poly.points_.size());
  std::generate(indices.begin(), indices.end(), IotaGenerator{});
  return std::any_of(indices.begin(), indices.end(), RightAngleChecker(poly));
}

size_t kuchukbaeva::getVertexCount(const Polygon& poly)
{
  return poly.points_.size();
}

bool kuchukbaeva::isEvenPolygon(const Polygon& poly)
{
  return poly.points_.size() % 2 == 0;
}

bool kuchukbaeva::isOddPolygon(const Polygon& poly)
{
  return poly.points_.size() % 2 != 0;
}

bool kuchukbaeva::hasNPoints(const Polygon& poly, size_t n)
{
  return poly.points_.size() == n;
}
