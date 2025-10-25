#include <iostream>
#include <boost/asio.hpp>
#include "common.h"
#include "bot.h"

using namespace nidaros::config;
using namespace nidaros::bot;

int main()
{
  print_welcome();

  try {

    boost::asio::io_context io_context;
    run(io_context);

  } catch (const std::exception& e) {
    std::cerr << "Failure: " << e.what() << "\n";
  }

  return 0;
}
