#include "commands.hpp"
#include "ioform.hpp"
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <functional>
#include <string>

namespace
{
  using iter_t = std::vector< int >::const_iterator;

  struct MaxSeqGenerator
  {
    iter_t current_;
    iter_t end_;

    MaxSeqGenerator(iter_t begin, iter_t end):
      current_(begin),
      end_(end)
    {}

    size_t operator()()
    {
      current_ = std::find(current_, end_, 1);
      if (current_ == end_)
      {
        return 0;
      }
      iter_t zero_it = std::find(current_, end_, 0);
      size_t len = std::distance(current_, zero_it);
      current_ = zero_it;
      return len;
    }
  };

  struct CompareLengths
  {
    bool operator()(size_t a, size_t b) const
    {
      return a < b;
    }
  };

  bool comparePolygons(const kuchukbaeva::Polygon& a, const kuchukbaeva::Polygon& b)
  {
    return a == b;
  }
}

void kuchukbaeva::areaCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  IOGuard guard(out);
  out << std::fixed << std::setprecision(1);
  if (param == "EVEN")
  {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isEvenPolygon);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
  else if (param == "ODD")
  {
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), isOddPolygon);
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
  else if (param == "MEAN")
  {
    if (polygons.empty())
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    const double sum = std::accumulate(areas.begin(), areas.end(), 0.0);
    out << sum / polygons.size() << "\n";
  }
  else
  {
    if (!std::all_of(param.begin(), param.end(), ::isdigit))
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    size_t n = std::stoull(param);
    if (n < 3)
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    std::vector< Polygon > filtered;
    std::copy_if(polygons.begin(), polygons.end(), std::back_inserter(filtered), std::bind(hasNPoints, std::placeholders::_1, n));
    std::vector< double > areas(filtered.size());
    std::transform(filtered.begin(), filtered.end(), areas.begin(), getArea);
    out << std::accumulate(areas.begin(), areas.end(), 0.0) << "\n";
  }
}

void kuchukbaeva::maxCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (param == "AREA")
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    const auto it = std::max_element(areas.begin(), areas.end());
    IOGuard guard(out);
    out << std::fixed << std::setprecision(1) << *it << "\n";
  }
  else if (param == "VERTEXES")
  {
    std::vector< size_t > counts(polygons.size());
    std::transform(polygons.begin(), polygons.end(), counts.begin(), getVertexCount);
    const auto it = std::max_element(counts.begin(), counts.end());
    out << *it << "\n";
  }
  else
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

void kuchukbaeva::minCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (polygons.empty())
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (param == "AREA")
  {
    std::vector< double > areas(polygons.size());
    std::transform(polygons.begin(), polygons.end(), areas.begin(), getArea);
    const auto it = std::min_element(areas.begin(), areas.end());
    IOGuard guard(out);
    out << std::fixed << std::setprecision(1) << *it << "\n";
  }
  else if (param == "VERTEXES")
  {
    std::vector< size_t > counts(polygons.size());
    std::transform(polygons.begin(), polygons.end(), counts.begin(), getVertexCount);
    const auto it = std::min_element(counts.begin(), counts.end());
    out << *it << "\n";
  }
  else
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }
}

void kuchukbaeva::countCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  std::string param;
  if (!(in >> param))
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  if (param == "EVEN")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isEvenPolygon) << "\n";
  }
  else if (param == "ODD")
  {
    out << std::count_if(polygons.begin(), polygons.end(), isOddPolygon) << "\n";
  }
  else
  {
    if (!std::all_of(param.begin(), param.end(), ::isdigit))
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    size_t n = std::stoull(param);
    if (n < 3)
    {
      out << "<INVALID COMMAND>\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    out << std::count_if(polygons.begin(), polygons.end(), std::bind(hasNPoints, std::placeholders::_1, n)) << "\n";
  }
}

void kuchukbaeva::maxseqCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  Polygon target;
  if (!(in >> target) || target.points_.size() < 3)
  {
    out << "<INVALID COMMAND>\n";
    in.clear();
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  if (in.peek() != '\n' && in.peek() != EOF)
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }

  std::vector< int > matches(polygons.size());
  std::transform(polygons.begin(), polygons.end(), matches.begin(), std::bind(comparePolygons, std::placeholders::_1, target));

  std::vector< size_t > sequences(matches.size());
  std::generate(sequences.begin(), sequences.end(), MaxSeqGenerator{matches.cbegin(), matches.cend()});

  const auto max_it = std::max_element(sequences.begin(), sequences.end(), CompareLengths{});
  size_t result = (max_it != sequences.end()) ? *max_it : 0;
  out << result << "\n";
}

void kuchukbaeva::rightshapesCommand(std::istream& in, std::ostream& out, const std::vector< Polygon >& polygons)
{
  if (in.peek() != '\n' && in.peek() != EOF)
  {
    out << "<INVALID COMMAND>\n";
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    return;
  }
  out << std::count_if(polygons.begin(), polygons.end(), hasRightAngle) << "\n";
}
