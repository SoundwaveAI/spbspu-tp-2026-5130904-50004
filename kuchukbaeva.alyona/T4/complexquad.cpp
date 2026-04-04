#include "complexquad.hpp"
#include <stdexcept>
#include <cmath>
#include <algorithm>

kuchukbaeva::Complexquad::Complexquad(point_t p1, point_t p2, point_t p3, point_t p4)
{
  pts_[0] = p1;
  pts_[1] = p2;
  pts_[2] = p3;
  pts_[3] = p4;
}

kuchukbaeva::point_t kuchukbaeva::Complexquad::getIntersection() const
{
  double a1 = pts_[2].y - pts_[0].y;
  double b1 = pts_[0].x - pts_[2].x;
  double c1 = a1 * pts_[0].x + b1 * pts_[0].y;

  double a2 = pts_[3].y - pts_[1].y;
  double b2 = pts_[1].x - pts_[3].x;
  double c2 = a2 * pts_[1].x + b2 * pts_[1].y;

  double det = a1 * b2 - a2 * b1;
  if (std::abs(det) < 1e-9) {
    throw std::logic_error("Segments do not intersect or are parallel");
  }

  return {(b2 * c1 - b1 * c2) / det, (a1 * c2 - a2 * c1) / det};
}

double kuchukbaeva::Complexquad::getArea() const
{
  point_t center = getIntersection();
  double area1 = std::abs((pts_[0].x * (pts_[1].y - center.y) +
                           pts_[1].x * (center.y - pts_[0].y) +
                           center.x * (pts_[0].y - pts_[1].y)) / 2.0);

  double area2 = std::abs((pts_[2].x * (pts_[3].y - center.y) +
                           pts_[3].x * (center.y - pts_[2].y) +
                           center.x * (pts_[2].y - pts_[3].y)) / 2.0);

  return area1 + area2;
}

kuchukbaeva::rectangle_t kuchukbaeva::Complexquad::getFrameRect() const
{
  double min_x = pts_[0].x;
  double max_x = pts_[0].x;
  double min_y = pts_[0].y;
  double max_y = pts_[0].y;

  for (int i = 1; i < 4; ++i) {
    min_x = std::min(min_x, pts_[i].x);
    max_x = std::max(max_x, pts_[i].x);
    min_y = std::min(min_y, pts_[i].y);
    max_y = std::max(max_y, pts_[i].y);
  }

  return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
}

void kuchukbaeva::Complexquad::move(point_t pos)
{
  point_t center = getIntersection();
  move(pos.x - center.x, pos.y - center.y);
}

void kuchukbaeva::Complexquad::move(double dx, double dy)
{
  for (int i = 0; i < 4; ++i) {
    pts_[i].x += dx;
    pts_[i].y += dy;
  }
}

void kuchukbaeva::Complexquad::scale(double cef)
{
  if (cef < 0.0) {
    throw std::invalid_argument("Scale coefficient must be positive");
  }
  point_t center = getIntersection();
  for (int i = 0; i < 4; ++i) {
    pts_[i].x = center.x + (pts_[i].x - center.x) * cef;
    pts_[i].y = center.y + (pts_[i].y - center.y) * cef;
  }
}
