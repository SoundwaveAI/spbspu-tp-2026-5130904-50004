#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include "dataStruct.hpp"

int main()
{
  std::vector< kuchukbaeva::DataStruct > data;
  using input_it_t = std::istream_iterator< kuchukbaeva::DataStruct >;

  std::copy(input_it_t{std::cin}, input_it_t{}, std::back_inserter(data));
  std::sort(data.begin(), data.end());

  using output_it_t = std::ostream_iterator< kuchukbaeva::DataStruct >;
  std::copy(data.begin(), data.end(), output_it_t{std::cout, "\n"});

  return 0;
}
