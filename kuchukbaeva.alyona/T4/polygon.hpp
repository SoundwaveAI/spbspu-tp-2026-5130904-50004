#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "shape.hpp"

namespace kuchukbaeva
{
  class Polygon: public Shape
  {
  public:
    Polygon(const std::vector< point_t >& vertices);
    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(point_t pos) override;
    void move(double dx, double dy) override;
    void scale(double cef) override;

  private:
    std::vector< point_t > vertices_;
    point_t getCentroid() const;
  };
}

#endif
