#include <curl_handler/curl_handler.hpp>

curl::curl_handler::curl_handler(const std::string & user_agent, on_write_sign on_write):
  __curl(curl_easy_init()),
  __user_agent(user_agent),
  __on_write(on_write)
{
  if (__curl == nullptr)
  {
    throw std::runtime_error("curl init failed!");
  }
}

curl::curl_handler::curl_handler(curl_handler && rhs):
  __curl(rhs.__curl),
  __user_agent(std::move(rhs.__user_agent))
{
  rhs.__curl = nullptr;
}

curl::curl_handler &
curl::curl_handler::operator=(curl_handler && rhs)
{
  if (std::addressof(rhs) == this)
  {
    return * this;
  }

  __curl = rhs.__curl;
  __user_agent = std::move(rhs.__user_agent);

  rhs.__curl = nullptr;

  return * this;
}

curl::curl_handler::~curl_handler()
{
  curl_easy_cleanup(__curl);
}
