#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <vector>
#include <memory>
#include "shape.hpp"

namespace kuchukbaeva
{
  void scaleRel(const std::vector< std::weak_ptr< Shape > >& shapes, point_t center, double k);
  rectangle_t getAggFrameRect(const std::vector< std::weak_ptr< Shape > >& shapes);
  double getAggArea(const std::vector< std::weak_ptr< Shape > >& shapes);
}

#endif
