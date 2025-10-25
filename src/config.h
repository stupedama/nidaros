#pragma once

#include <string>
#include <string_view>
#include <fstream>
#include "common.h"

namespace nidaros::config {

// handles the reading of the config file
class Config
{
public:
  Config();
  explicit Config(std::string_view config_file);
  const common::config_container& get_config() const;
  const common::channels_container& get_channels() const;
private:
  void read_file();
  std::string trim_whitespace(const std::string& str) const;
private:
  std::string config_file;
  common::config_container config;
  common::channels_container channels;
};

} // namespace
