#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "shape.hpp"
#include <istream>
#include <ostream>
#include <vector>

namespace kuchukbaeva
{
  void areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void maxseqCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
  void rightshapesCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons);
}

#endif
