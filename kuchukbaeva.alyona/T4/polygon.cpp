#include "polygon.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>

kuchukbaeva::Polygon::Polygon(const std::vector<point_t>& vertices):
  vertices_(vertices)
{
  if (vertices_.size() < 3) {
    throw std::invalid_argument("Polygon must have at least 3 vertices");
  }
}

double kuchukbaeva::Polygon::getArea() const
{
  double area = 0.0;
  size_t n = vertices_.size();
  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    area += (vertices_[i].x * vertices_[j].y - vertices_[j].x * vertices_[i].y);
  }
  return std::abs(area) / 2.0;
}

kuchukbaeva::rectangle_t kuchukbaeva::Polygon::getFrameRect() const
{
  double min_x = vertices_[0].x;
  double max_x = vertices_[0].x;
  double min_y = vertices_[0].y;
  double max_y = vertices_[0].y;

  for (size_t i = 1; i < vertices_.size(); ++i) {
    min_x = std::min(min_x, vertices_[i].x);
    max_x = std::max(max_x, vertices_[i].x);
    min_y = std::min(min_y, vertices_[i].y);
    max_y = std::max(max_y, vertices_[i].y);
  }
  return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
}

kuchukbaeva::point_t kuchukbaeva::Polygon::getCentroid() const
{
  double cx = 0.0;
  double cy = 0.0;
  double area = 0.0;
  size_t n = vertices_.size();

  for (size_t i = 0; i < n; ++i) {
    size_t j = (i + 1) % n;
    double cross = (vertices_[i].x * vertices_[j].y - vertices_[j].x * vertices_[i].y);
    cx += (vertices_[i].x + vertices_[j].x) * cross;
    cy += (vertices_[i].y + vertices_[j].y) * cross;
    area += cross;
  }

  area /= 2.0;
  cx /= (6.0 * area);
  cy /= (6.0 * area);

  return {cx, cy};
}

void kuchukbaeva::Polygon::move(point_t pos)
{
  point_t centroid = getCentroid();
  move(pos.x - centroid.x, pos.y - centroid.y);
}

void kuchukbaeva::Polygon::move(double dx, double dy)
{
  for (size_t i = 0; i < vertices_.size(); ++i) {
    vertices_[i].x += dx;
    vertices_[i].y += dy;
  }
}

void kuchukbaeva::Polygon::scale(double cef)
{
  if (cef < 0.0) {
    throw std::invalid_argument("Scale cef must be positive");
  }
  point_t centroid = getCentroid();
  for (size_t i = 0; i < vertices_.size(); ++i) {
    vertices_[i].x = centroid.x + (vertices_[i].x - centroid.x) * cef;
    vertices_[i].y = centroid.y + (vertices_[i].y - centroid.y) * cef;
  }
}
