#include <iostream>

#include <curlxx/handler.hpp>
#include <curlxx/json.hpp>

int main(int argc, char ** argv)
{
  try
  {
    // GET REQUEST
    {
      curlxx::params get_pm = {
        .url = "https://catfact.ninja/fact",
        .user_agent = "firefox"
      };

      auto answer = curlxx::get< nlohmann::json >(get_pm);
      std::cout << answer.dump(2) << std::endl;
    }
    // ASYNC GET REQUEST
    {
      curlxx::params async_get_pm = {
        .url = "https://catfact.ninja/fact",
        .user_agent = "firefox"
      };

      auto answer = curlxx::async_get< nlohmann::json >(async_get_pm);
      std::cout << answer.get().dump(2) << std::endl;
    }
    // POST REQUEST
    {
      curlxx::params post_pm = {
        .url = "https://httpbin.org/post",
        .query = nlohmann::json{"post", "test"},
        .user_agent = "firefox"
      };

      auto answer = curlxx::post< nlohmann::json >(post_pm);
      std::cout << answer.dump(2) << std::endl;
    }
    // ASYNC POST REQUEST
    {
      curlxx::params async_post_pm = {
        .url = "https://httpbin.org/post",
        .query = nlohmann::json{"async", "post", "test"},
        .user_agent = "firefox"
      };

      auto answer = curlxx::async_post< nlohmann::json >(async_post_pm);
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
