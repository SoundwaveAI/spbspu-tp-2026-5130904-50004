#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <vector>
#include <istream>
#include <ostream>

namespace kuchukbaeva
{
  struct Point
  {
    int x_;
    int y_;
  };

  bool operator==(const Point& lhs, const Point& rhs);

  std::istream& operator>>(std::istream& in, Point& dest);
  std::ostream& operator<<(std::ostream& out, const Point& src);

  struct Polygon
  {
    std::vector< Point > points_;
  };

  bool operator==(const Polygon& lhs, const Polygon& rhs);

  std::istream& operator>>(std::istream& in, Polygon& dest);
  std::ostream& operator<<(std::ostream& out, const Polygon& src);

  double getArea(const Polygon& poly);
  bool hasRightAngle(const Polygon& poly);
  size_t getVertexCount(const Polygon& poly);

  bool isEvenPolygon(const Polygon& poly);
  bool isOddPolygon(const Polygon& poly);
  bool hasNPoints(const Polygon& poly, size_t n);
}

#endif
