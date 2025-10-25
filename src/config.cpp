#include "config.h"
#include <sstream>

namespace nidaros::config
{

Config::Config() : config_file{"nidaros.conf"}
{
  read_file();
}

Config::Config(std::string_view config_file) : config_file{config_file}
{
  read_file();
}

const common::config_container& Config::get_config() const
{
  return config;
}

const common::channels_container& Config::get_channels() const
{
  return channels;
}

std::string Config::trim_whitespace(const std::string& str) const
{
  std::size_t first = str.find_first_not_of(" \t\n\r");

  if(std::string::npos == first) {
    return str;
  }

  std::size_t last = str.find_last_not_of(" \t\n\r");

  return str.substr(first, (last - first +1));
}

void Config::read_file()
{
  std::ifstream file(config_file);

  if(!file.is_open()) {
    throw std::runtime_error("Failed to open config file: " + config_file);
  }

  std::string config_line;

  while(std::getline(file, config_line)) {
    if(config_line.empty() || config_line[0] == '#') {
      continue;
    }

    std::size_t delim_pos = config_line.find('=');
    if(delim_pos != std::string::npos) {
      std::string key = config_line.substr(0, delim_pos);
      std::string value = config_line.substr(delim_pos +1);

      // trim whitespace
      key = trim_whitespace(key);
      value = trim_whitespace(value);

      if(key != "channels") {
        config[key] = value;
      } else {
        std::string line = value;
        std::stringstream ss(line);
        std::string channel;

        while(std::getline(ss, channel, ','))
        {
          if(!channel.empty())
          {
            channels.push_back(channel);
          }
        }
      }
    }
  }

  // config should be
  if(config.size() < common::CONFIG_SIZE) {
    throw std::runtime_error("Missing values in file: " + config_file);
  }

  file.close();
}

} // namespace nidaros::config
