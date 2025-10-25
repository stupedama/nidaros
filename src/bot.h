#pragma once

#include <iostream>
#include <asio.hpp>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include "common.h"
#include "config.h"
#include "common.h"

using asio::ip::tcp;
using namespace nidaros::common;
using namespace nidaros::config;

namespace nidaros::bot
{

class Bot
{
public:
  Bot(tcp::socket& socket, std::string nickname, std::string ident, std::string name, common::channels_container channels)
  : socket{socket}, nickname{std::move(nickname)}, ident{std::move(ident)}, name{std::move(name)}, channels{std::move(channels)}
  {
    set_nickname();
    set_ident();
  }
  Bot(const Bot&) = delete;
  ~Bot();
  void run(std::atomic<bool>& connected) const;
  void join() const;
private:
  void set_nickname() const;
  void set_ident() const;
  void quit() const;
private:
  tcp::socket& socket;
  std::string nickname;
  std::string ident;
  std::string name;
  common::channels_container channels;
};

// running the program
void run(asio::io_context& io_context);

} // namespace

