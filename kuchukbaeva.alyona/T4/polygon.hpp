#ifndef POLYGON_HPP
#define POLYGON_HPP
#include "shape.hpp"
#include <vector>

namespace kuchukbaeva {

  class Polygon : public Shape {
  public:
    Polygon(const std::vector<point_t>& points);

    double getArea() const override;
    rectangle_t getFrameRect() const override;
    void move(const point_t& pos) override;
    void move(double dx, double dy) override;
    void scale(double cef) override;

  private:
    std::vector<point_t> points_;
    point_t getCenter() const;
  };

}

#endif
