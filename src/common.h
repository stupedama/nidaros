#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>

namespace nidaros::common
{

// aliases
using config_container = std::unordered_map<std::string,std::string>;
using channels_container = std::vector<std::string>;

// constants
constexpr int CONFIG_SIZE{5};

} // namespace

namespace nidaros::common
{
void print_welcome();
} // namespace
