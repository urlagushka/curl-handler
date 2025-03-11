#include <curlxx/handler.hpp>

curlxx::handler::handler(const std::string & user_agent, on_write_sign on_write):
  __user_agent(user_agent),
  __on_write(on_write),
  __is_debug(false)
{
  curl_global_init(CURL_GLOBAL_DEFAULT);
}

/*
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
*/

curlxx::handler::~handler()
{
  curl_global_cleanup();
}

void curlxx::handler::set_debug_state(bool rhs)
{
  __is_debug = rhs;
}
