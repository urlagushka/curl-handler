#include <iostream>

#include <curl_handler/curl_handler.hpp>
#include <curl_handler/json.hpp>

int main(int argc, char ** argv)
{
  try
  {
    curl::curl_handler qq("firefox");
    // qq.set_debug_state(true);

    // GET REQUEST
    {
      auto answer = qq.get< nlohmann::json >("https://catfact.ninja/fact");
      std::cout << answer.dump(2) << std::endl;
    }

    // ASYNC GET REQUEST
    {
      auto answer = qq.async_get< nlohmann::json >("https://catfact.ninja/fact");
      std::cout << answer.get().dump(2) << std::endl;
    }

    // POST REQUEST
    {
      auto answer = qq.post< nlohmann::json >("'https://dummyjson.com/posts'", {});
      std::cout << answer.dump(2) << std::endl;
    }

    // ASYNC POST REQUEST
    {
      auto answer = qq.async_post< nlohmann::json >("'https://dummyjson.com/posts'", {});
      std::cout << answer.get().dump(2) << std::endl;
    }
  }
  catch (const std::runtime_error & error)
  {
    std::cout << error.what() << std::endl;
  }
  catch (const nlohmann::json::parse_error & error)
  {
    std::cout << error.what() << std::endl;
  }
  return 0;
}
