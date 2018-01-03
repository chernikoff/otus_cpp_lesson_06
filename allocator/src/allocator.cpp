#include <iostream>
#include <stdexcept>

int main(int /*argc*/, char const * /*argv*/[])
{
  try
  {
  }
  catch(std::exception const & e)
  {
    std::cerr << e.what() << std::endl;
    throw;
  }

  return 0;
}
