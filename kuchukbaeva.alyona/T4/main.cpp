#include <iostream>
#include <memory>
#include <vector>
#include "rectangle.hpp"
#include "polygon.hpp"
#include "complexquad.hpp"
#include "functions.hpp"

void printS(const std::vector< std::weak_ptr< kuchukbaeva::Shape > >& shapes)
{
  using namespace kuchukbaeva;
  std::cout << "Aggregate Area: " << getAggArea(shapes) << "\n";
  rectangle_t agg_frame = getAggFrameRect(shapes);
  std::cout << "Aggregate Frame: pos(" << agg_frame.pos.x << ", " << agg_frame.pos.y
            << ") w:" << agg_frame.width << " h:" << agg_frame.height << "\n";

  for (size_t i = 0; i < shapes.size(); ++i)
  {
    if (auto shape = shapes[i].lock())
    {
      rectangle_t frame = shape->getFrameRect();
      std::cout << "Shape " << i << " Area: " << shape->getArea() << " | "
                << "Frame pos(" << frame.pos.x << ", " << frame.pos.y
                << ") w:" << frame.width << " h:" << frame.height << "\n";
    }
  }
  std::cout << " ";
}

int main()
{
  using namespace kuchukbaeva;
  try
  {
    std::vector< std::shared_ptr< Shape > > shared_shapes;
    shared_shapes.push_back(std::make_shared< Rectangle >(point_t{5.0, 5.0}, 4.0, 2.0));

    std::vector< point_t > poly_pts = {{0.0, 0.0}, {4.0, 0.0}, {2.0, 4.0}};
    shared_shapes.push_back(std::make_shared< Polygon >(poly_pts));
    shared_shapes.push_back(std::make_shared< Complexquad >(point_t{0.0, 0.0},
      point_t{4.0, 0.0}, point_t{4.0, 4.0}, point_t{0.0, 4.0}
    ));

    std::vector< std::weak_ptr< Shape > > weak_shapes;
    for (size_t i = 0; i < shared_shapes.size(); ++i)
    {
      weak_shapes.push_back(shared_shapes[i]);
    }

    std::cout << "BEFORE SCALING" << "\n";
    printS(weak_shapes);

    double x = 0.0;
    double y = 0.0;
    double k = 0.0;
    if (!(std::cin >> x >> y >> k))
    {
      return 0;
    }

    scaleRel(weak_shapes, {x, y}, k);

    std::cout << "AFTER SCALING" << "\n";
    printS(weak_shapes);

  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 2;
  }
  return 0;
}
