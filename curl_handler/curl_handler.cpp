#include <curl_handler/curl_handler.hpp>

curl::curl_handler::curl_handler(const std::string & user_agent, on_write_sign on_write):
  __user_agent(user_agent),
  __on_write(on_write),
  __is_debug(false)
{}

curl::curl_handler::curl_handler(curl_handler && rhs):
  __user_agent(std::move(rhs.__user_agent)),
  __on_write(rhs.__on_write),
  __is_debug(rhs.__is_debug)
{}

curl::curl_handler &
curl::curl_handler::operator=(curl_handler && rhs)
{
  if (std::addressof(rhs) == this)
  {
    return * this;
  }

  __user_agent = std::move(rhs.__user_agent);
  __on_write = rhs.__on_write;
  __is_debug = rhs.__is_debug;

  return * this;
}

curl::curl_handler::~curl_handler()
{
  curl_global_cleanup();
}

void curl::curl_handler::set_debug_state(bool rhs)
{
  __is_debug = rhs;
}
