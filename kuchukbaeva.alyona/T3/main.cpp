#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <string>
#include <limits>
#include <stdexcept>
#include "shape.hpp"
#include "commands.hpp"

void kuchukbaeva::readPolygonsRecursive(std::istream& in, std::vector< Polygon >& polygons)
{
  if (!in || in.eof())
  {
    return;
  }
  Polygon p;
  if (in >> p)
  {
    polygons.push_back(p);
  }
  else if (!in.eof())
  {
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
  readPolygonsRecursive(in, polygons);
}

void kuchukbaeva::processCommandRecursive(std::istream& in, std::ostream& out,
  const std::map< std::string, const_cmd_t >& const_cmds,
  const std::map< std::string, non_const_cmd_t >& non_const_cmds)
{
  if (in.eof())
  {
    return;
  }

  std::string command;
  if (!(in >> command))
  {
    return;
  }

  auto const_it = const_cmds.find(command);
  if (const_it != const_cmds.end())
  {
    const_it->second(in, out);
  }
  else
  {
    auto non_const_it = non_const_cmds.find(command);
    if (non_const_it != non_const_cmds.end())
    {
      non_const_it->second(in, out);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  processCommandRecursive(in, out, const_cmds, non_const_cmds);
}

int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    std::cerr << "Invalid number of arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  std::vector< kuchukbaeva::Polygon > polygons;
  kuchukbaeva::readPolygonsRecursive(file, polygons);

  std::map< std::string, kuchukbaeva::const_cmd_t > const_cmds;
  std::map< std::string, kuchukbaeva::non_const_cmd_t > non_const_cmds;

  using namespace std::placeholders;

  const_cmds["AREA"] = std::bind(kuchukbaeva::areaCommand, _1, _2, std::cref(polygons));
  const_cmds["MAX"] = std::bind(kuchukbaeva::maxCommand, _1, _2, std::cref(polygons));
  const_cmds["MIN"] = std::bind(kuchukbaeva::minCommand, _1, _2, std::cref(polygons));
  const_cmds["COUNT"] = std::bind(kuchukbaeva::countCommand, _1, _2, std::cref(polygons));
  const_cmds["MAXSEQ"] = std::bind(kuchukbaeva::maxseqCommand, _1, _2, std::cref(polygons));
  const_cmds["RIGHTSHAPES"] = std::bind(kuchukbaeva::rightshapesCommand, _1, _2, std::cref(polygons));

  kuchukbaeva::processCommandRecursive(std::cin, std::cout, const_cmds, non_const_cmds);

  return 0;
}
