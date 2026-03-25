#include "functions.hpp"
#include <stdexcept>
#include <algorithm>

void kuchukbaeva::scaleRel(const std::vector<std::weak_ptr<Shape>>& shapes, point_t center, double k)
{
  if (k < 0.0) {
    throw std::invalid_argument("Cef must be non-negative");
  }
  for (const auto& weak_shape : shapes) {
    if (auto shape = weak_shape.lock()) {
      point_t shape_center = shape->getFrameRect().pos;
      double dx = shape_center.x - center.x;
      double dy = shape_center.y - center.y;
      shape->move({center.x + dx * k, center.y + dy * k});
      shape->scale(k);
    }
  }
}

kuchukbaeva::rectangle_t kuchukbaeva::getAggFrameRect(const std::vector<std::weak_ptr<Shape>>& shapes)
{
  if (shapes.empty()) {
    return {0.0, 0.0, {0.0, 0.0}};
  }

  bool first = true;
  double min_x = 0, min_y = 0, max_x = 0, max_y = 0;

  for (const auto& weak_shape : shapes) {
    if (auto shape = weak_shape.lock()) {
      rectangle_t frame = shape->getFrameRect();
      double cur_min_x = frame.pos.x - frame.width / 2.0;
      double cur_max_x = frame.pos.x + frame.width / 2.0;
      double cur_min_y = frame.pos.y - frame.height / 2.0;
      double cur_max_y = frame.pos.y + frame.height / 2.0;

      if (first) {
        min_x = cur_min_x;
        max_x = cur_max_x;
        min_y = cur_min_y;
        max_y = cur_max_y;
        first = false;
      } else {
        min_x = std::min(min_x, cur_min_x);
        max_x = std::max(max_x, cur_max_x);
        min_y = std::min(min_y, cur_min_y);
        max_y = std::max(max_y, cur_max_y);
      }
    }
  }

  return {max_x - min_x, max_y - min_y, {(min_x + max_x) / 2.0, (min_y + max_y) / 2.0}};
}

double kuchukbaeva::getAggArea(const std::vector<std::weak_ptr<Shape>>& shapes)
{
  double total_area = 0.0;
  for (const auto& weak_shape : shapes) {
    if (auto shape = weak_shape.lock()) {
      total_area += shape->getArea();
    }
  }
  return total_area;
}
