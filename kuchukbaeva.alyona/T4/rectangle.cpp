#include "rectangle.hpp"
#include <stdexcept>

kuchukbaeva::Rectangle::Rectangle(point_t pos, double width, double height):
  pos_(pos),
  width_(width),
  height_(height)
{
  if (width_ < 0.0 || height_ < 0.0)
  {
    throw std::invalid_argument("Rectangle dimensions must be positive");
  }
}

double kuchukbaeva::Rectangle::getArea() const
{
  return width_ * height_;
}

kuchukbaeva::rectangle_t kuchukbaeva::Rectangle::getFrameRect() const
{
  return {width_, height_, pos_};
}

void kuchukbaeva::Rectangle::move(point_t pos)
{
  pos_ = pos;
}

void kuchukbaeva::Rectangle::move(double dx, double dy)
{
  pos_.x += dx;
  pos_.y += dy;
}

void kuchukbaeva::Rectangle::scale(double cef)
{
  if (cef < 0.0)
  {
    throw std::invalid_argument("Scale cef must be positive");
  }
  width_ *= cef;
  height_ *= cef;
}
