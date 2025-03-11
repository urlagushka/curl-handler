#ifndef CURL_HANDLER_HPP
#define CURL_HANDLER_HPP

#include <string>
#include <utility>
#include <format>
#include <future>
#include <thread>
#include <string_view>

#include <curl/curl.h>
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

namespace curl
{
  using url_t = std::string_view;
  using query_t = nlohmann::json;
  using on_write_sign = std::size_t (*)(char * , std::size_t, std::size_t, std::string &);

  class curl_handler
  {
    public:
      curl_handler(const std::string & user_agent, on_write_sign on_write = default_on_write);
      curl_handler(const curl_handler & rhs) = delete;
      curl_handler(curl_handler && rhs);
      curl_handler & operator=(const curl_handler & rhs) = delete;
      curl_handler & operator=(curl_handler && rhs);
      ~curl_handler();

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

      CURL * __curl;
      std::string __user_agent;
      on_write_sign __on_write;
      bool __is_debug;
  };
}

template < correct_answer_t answer_t >
answer_t
curl::curl_handler::post(url_t url, const query_t & query)
{
  if (__curl == nullptr)
  {
    throw std::runtime_error("curl is null!");
  }
  if (url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  std::string response;
  std::string tmp = query.dump();

  if (__is_debug)
  {
    curl_easy_setopt(__curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(__curl, CURLOPT_URL, url.data());
  curl_easy_setopt(__curl, CURLOPT_POST, 1L);
  curl_easy_setopt(__curl, CURLOPT_POSTFIELDS, tmp.c_str());
  curl_easy_setopt(__curl, CURLOPT_POSTFIELDSIZE, static_cast< long >(tmp.size()));

  curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, __on_write);
  curl_easy_setopt(__curl, CURLOPT_WRITEDATA, &response);

  curl_slist * headers = nullptr;
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, std::format("User-Agent: {}", __user_agent).c_str());
  curl_easy_setopt(__curl, CURLOPT_HTTPHEADER, headers);

  CURLcode code = curl_easy_perform(__curl);

  curl_slist_free_all(headers);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(std::format("POST {} ERROR\n{}", url, curl_easy_strerror(code)));
  }
  
  return string_to_answer< answer_t >(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curl::curl_handler::async_post(url_t url, const query_t & query)
{
  return std::async(std::launch::async, &curl_handler::post< answer_t >, this, url, query);
}

template < correct_answer_t answer_t >
answer_t
curl::curl_handler::get(url_t url)
{
  if (__curl == nullptr)
  {
    throw std::runtime_error("curl is null!");
  }
  if (url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  std::string response;

  if (__is_debug)
  {
    curl_easy_setopt(__curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(__curl, CURLOPT_URL, url.data());
  curl_easy_setopt(__curl, CURLOPT_HTTPGET, 1L);

  curl_easy_setopt(__curl, CURLOPT_WRITEFUNCTION, __on_write);
  curl_easy_setopt(__curl, CURLOPT_WRITEDATA, &response);

  curl_slist * headers = nullptr;
  headers = curl_slist_append(headers, std::format("User-Agent: {}", __user_agent).c_str());
  curl_easy_setopt(__curl, CURLOPT_HTTPHEADER, headers);

  CURLcode code = curl_easy_perform(__curl);

  curl_slist_free_all(headers);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(std::format("GET {} ERROR\n{}", url, curl_easy_strerror(code)));
  }

  return string_to_answer< answer_t >(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curl::curl_handler::async_get(url_t url)
{
  return std::async(std::launch::async, &curl_handler::get< answer_t >, this, url);
}

template < correct_answer_t answer_t >
answer_t
curl::curl_handler::string_to_answer(const std::string & rhs)
{
  // need rework func
  answer_t answer = nlohmann::json::parse(rhs);
  return answer;
}

#endif
