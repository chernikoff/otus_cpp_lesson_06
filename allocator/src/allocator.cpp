#include <iostream>
#include <stdexcept>
#include <map>
#include <cassert>

#include <allocator/pool_allocator.h>
#include <allocator/vector.h>

auto factorial(size_t num)
{
  size_t result = 1;
  for(size_t i = 1; i <= num; ++i) {
    result *= i;
  }
  return result;
}

int main(int /*argc*/, char const * /*argv*/[])
{
  try {
    auto initialize_map = [](auto & map) {
      for(size_t i = 0; i < 10; ++i) {
        map.emplace(i, factorial(i));
      }
    };

    {
      std::map< size_t, size_t > map_default_alloc;
      initialize_map(map_default_alloc);
      assert(map_default_alloc.size() == 10);
    }

    {
      std::map< size_t, size_t, std::less<>,
                pool_allocator< std::pair< size_t const, size_t >, 10 >
                > map_custom_alloc;
      initialize_map(map_custom_alloc);
      assert(map_custom_alloc.size() == 10);

      for (auto begin = std::begin(map_custom_alloc), end = std::end(map_custom_alloc), it = begin;
           it != end; ++it) {
        std::cout << it->first << ' ' << it->second << '\n';
      }
    }

    auto initialize_vector = [](auto & vector) {
      for (size_t i = 0; i < 10; ++i) {
        vector.push_back(i);
      }
    };

    {
      vector< size_t > my_vector;
      initialize_vector(my_vector);
    }

    {
      vector< size_t, pool_allocator< size_t, 10 > > my_vector;
      initialize_vector(my_vector);

      for (auto begin = std::begin(my_vector), end = std::end(my_vector), it = begin;
           it != end; ++it) {
        std::cout << *it << '\n';
      }
    }

  }
  catch(std::exception const & e) {
    std::cerr << e.what() << std::endl;
    throw;
  }


  std::cout.flush();
  return 0;
}
