#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <utility>
#include <format>
#include <future>
#include <thread>
#include <string_view>

#include <curl/curl.h>

#include "utils.hpp"
#include "json.hpp"

namespace
{
  static std::size_t
  default_on_write(char * ptr, std::size_t size, std::size_t nmemb, std::string & userdata)
  {
    userdata.append(ptr, size * nmemb);
    return size * nmemb;
  }
}

template < typename answer_t >
concept correct_answer_t = requires(answer_t answer, const std::string & rhs)
{
  { answer = rhs } -> std::same_as< answer_t & >;
};

namespace curlxx
{
  using url_t = std::string_view;
  using query_t = nlohmann::json;
  using on_write_sign = std::size_t (*)(char * , std::size_t, std::size_t, std::string &);

  class handler
  {
    public:
      handler(const std::string & user_agent, on_write_sign on_write = default_on_write);
      handler(const handler & rhs) = default;
      handler(handler && rhs) = default;
      handler & operator=(const handler & rhs) = default;
      handler & operator=(handler && rhs) = default;
      ~handler();

      void set_debug_state(bool rhs);

      template < correct_answer_t answer_t >
      answer_t post(url_t url, const query_t & query);

      template < correct_answer_t answer_t >
      std::future< answer_t > async_post(url_t url, const query_t & query);

      template < correct_answer_t answer_t >
      answer_t get(url_t url);

      template < correct_answer_t answer_t >
      std::future< answer_t > async_get(url_t url);

    private:
      template < correct_answer_t answer_t >
      answer_t string_to_answer(const std::string & rhs);

      std::string __user_agent;
      on_write_sign __on_write;
      bool __is_debug;
  };
}

template < correct_answer_t answer_t >
answer_t
curlxx::handler::post(url_t url, const query_t & query)
{
  if (url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd req;
  std::string response;
  std::string tmp = query.dump();

  if (__is_debug)
  {
    curl_easy_setopt(req.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(req.curl, CURLOPT_URL, url.data());
  curl_easy_setopt(req.curl, CURLOPT_POST, 1L);
  curl_easy_setopt(req.curl, CURLOPT_POSTFIELDS, tmp.c_str());
  curl_easy_setopt(req.curl, CURLOPT_POSTFIELDSIZE, static_cast< long >(tmp.size()));

  curl_easy_setopt(req.curl, CURLOPT_WRITEFUNCTION, __on_write);
  curl_easy_setopt(req.curl, CURLOPT_WRITEDATA, &response);

  utils::slist_fd headers;
  headers.append("Content-Type: application/json");
  headers.append(std::format("User-Agent: {}", __user_agent).c_str());
  curl_easy_setopt(req.curl, CURLOPT_HTTPHEADER, headers.slist);

  CURLcode code = curl_easy_perform(req.curl);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(std::format("POST {} ERROR\n{}", url, curl_easy_strerror(code)));
  }

  return string_to_answer< answer_t >(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curlxx::handler::async_post(url_t url, const query_t & query)
{
  return std::async(std::launch::async, &handler::post< answer_t >, this, url, query);
}

template < correct_answer_t answer_t >
answer_t
curlxx::handler::get(url_t url)
{
  if (url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd req;
  std::string response;

  if (__is_debug)
  {
    curl_easy_setopt(req.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(req.curl, CURLOPT_URL, url.data());
  curl_easy_setopt(req.curl, CURLOPT_HTTPGET, 1L);

  curl_easy_setopt(req.curl, CURLOPT_WRITEFUNCTION, __on_write);
  curl_easy_setopt(req.curl, CURLOPT_WRITEDATA, &response);

  utils::slist_fd headers;
  headers.append(std::format("User-Agent: {}", __user_agent).c_str());
  curl_easy_setopt(req.curl, CURLOPT_HTTPHEADER, headers.slist);

  CURLcode code = curl_easy_perform(req.curl);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(std::format("GET {} ERROR\n{}", url, curl_easy_strerror(code)));
  }

  return string_to_answer< answer_t >(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curlxx::handler::async_get(url_t url)
{
  return std::async(std::launch::async, &handler::get< answer_t >, this, url);
}

template < correct_answer_t answer_t >
answer_t
curlxx::handler::string_to_answer(const std::string & rhs)
{
  // need rework func
  answer_t answer = nlohmann::json::parse(rhs);
  return answer;
}

#endif
