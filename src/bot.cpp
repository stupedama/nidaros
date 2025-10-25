#include "bot.h"
#include "common.h"

namespace nidaros::bot
{

Bot::~Bot()
{
  quit();
}

void Bot::run(std::atomic<bool>& connected) const
{
  asio::streambuf buffer;
  std::istream is(&buffer);

  try {
    while(true)
    {
      asio::read_until(socket, buffer, "\r\n");

      std::string line;
      std::getline(is, line);

      if(!line.empty() && line.back() == '\r')
      {
        line.pop_back();
      }

      std::cout << line << std::endl;

      // respond to server to ping messages
      if(line.rfind("PING", 0) == 0)
      {
        std::string response{"PONG " + line.substr(5) + "\r\n"};
        asio::write(socket, asio::buffer(response));
        std::cout << response << std::endl;
      }

      if(line.find(" 001 ") != std::string::npos) {
        connected = true;
      }

      if(line.find(" 433 ") != std::string::npos) {
        if(nickname.size() > NICKNAME_MAXCHARS) {
          quit();
          // give time to break out of loop
          std::this_thread::sleep_for(std::chrono::seconds(2));
          throw std::runtime_error("Nickname longer than allowed characters.");
        } else {
          nickname = nickname + "^";
          set_nickname();
        }
      }

      if(line.find(" 403 ") != std::string::npos) {
        std::cerr << line << std::endl;
      }
    }
  }
    catch (const std::exception& e)
    {
      std::cerr << "IRC read loop ended: " << e.what() << '\n';
      throw e;
    }
}

void Bot::set_nickname() const
{
    std::string nick_cmd = "NICK " + nickname + "\r\n";
    asio::write(socket, asio::buffer(nick_cmd));
}

void Bot::set_ident() const
{
    std::string user_cmd = "USER " + ident + " 0 * :" + name + "\r\n";
    asio::write(socket, asio::buffer(user_cmd));
}

void Bot::join() const
{
  for(const auto& c : channels)
  {
    std::string join_cmd = "JOIN #" + c + "\r\n";
    asio::write(socket, asio::buffer(join_cmd));
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

void Bot::quit() const
{
  std::string quit_cmd = "QUIT :nidaros v0.1.0\r\n";
  asio::write(socket, asio::buffer(quit_cmd));
  socket.close();
}

// handles starting the bot
void run(asio::io_context& io_context)
{
  Config config_file;
  auto config = config_file.get_config();

  tcp::resolver resolver(io_context);
  auto endpoints = resolver.resolve(config["host"], config["port"]);

  // connect
  tcp::socket socket(io_context);
  asio::connect(socket, endpoints);

  std::cout << "Connecting to " << config["host"] << " on port " << config["port"] << std::endl;

  auto channels = config_file.get_channels();
  bot::Bot bot(socket, config["nickname"], config["ident"], config["name"], std::move(channels));

  std::atomic<bool> connected{false};
  std::thread reader([&socket,&connected,&bot]() { bot.run(connected); });

  while(!connected) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  // join channels
  bot.join();

  reader.join();
}

} // namespace
