#ifndef CURLXX_HPP
#define CURLXX_HPP

#include <string>
#include <format>
#include <future>
#include <thread>
#include <optional>
#include <string_view>

#include <curl/curl.h>

#include "utils.hpp"
#include "json.hpp"

namespace
{
  inline std::size_t
  default_on_write(char * ptr, std::size_t size, std::size_t nmemb, std::string & userdata)
  {
    userdata.append(ptr, size * nmemb);
    return size * nmemb;
  }

  template < typename answer_t >
  answer_t
  from_string(std::string_view rhs)
  {
    return answer_t(rhs);
  }

  template < >
  nlohmann::json
  from_string(std::string_view rhs)
  {
    return nlohmann::json::parse(rhs);
  }
}

namespace curlxx
{
  using on_write_sign = std::size_t (*)(char * , std::size_t, std::size_t, std::string &);

  struct params
  {
    std::string url;
    std::optional< nlohmann::json > query;
    std::optional< std::string > user_agent;

    std::optional< on_write_sign > on_write;
    std::optional< bool > is_debug;
  };

  template < typename answer_t >
  answer_t
  post(const params & pm);

  template < typename answer_t >
  std::future< answer_t >
  async_post(const params & pm);

  template < typename answer_t >
  answer_t
  get(const params & pm);

  template < typename answer_t >
  std::future< answer_t >
  async_get(const params & pm);
}

template < typename answer_t >
answer_t
curlxx::post(const params & pm)
{
  if (pm.url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd curl_ex;
  utils::slist_fd headers;
  std::string response;
  std::string user_agent;
  std::string query_dump;

  curl_easy_setopt(curl_ex.curl, CURLOPT_SSL_VERIFYPEER, 1L);
  curl_easy_setopt(curl_ex.curl, CURLOPT_SSL_VERIFYHOST, 2L);
  if (pm.is_debug.has_value() && pm.is_debug.value())
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_URL, pm.url.c_str());
  curl_easy_setopt(curl_ex.curl, CURLOPT_POST, 1L);
  if (pm.query.has_value())
  {
    query_dump = pm.query.value().dump();
    curl_easy_setopt(curl_ex.curl, CURLOPT_POSTFIELDS, query_dump.c_str());
    curl_easy_setopt(curl_ex.curl, CURLOPT_POSTFIELDSIZE, query_dump.size());
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, (pm.on_write.has_value()) ? pm.on_write.value() : default_on_write);
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEDATA, &response);

  headers.append("Content-Type: application/json");
  if (pm.user_agent.has_value())
  {
    user_agent = std::format("User-Agent: {}", pm.user_agent.value());
    headers.append(user_agent.c_str());
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_HTTPHEADER, headers.slist);

  CURLcode code = curl_easy_perform(curl_ex.curl);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(curl_easy_strerror(code));
  }

  return from_string< answer_t >(response);
}

template < typename answer_t >
std::future< answer_t >
curlxx::async_post(const params & pm)
{
  return std::async(std::launch::async, &post< answer_t >, std::cref(pm));
}

template < typename answer_t >
answer_t
curlxx::get(const params & pm)
{
  if (pm.url.empty())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd curl_ex;
  utils::slist_fd headers;
  std::string response;
  std::string user_agent;

  curl_easy_setopt(curl_ex.curl, CURLOPT_SSL_VERIFYPEER, 1L);
  curl_easy_setopt(curl_ex.curl, CURLOPT_SSL_VERIFYHOST, 2L);
  if (pm.is_debug.has_value() && pm.is_debug.value())
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_URL, pm.url.c_str());
  curl_easy_setopt(curl_ex.curl, CURLOPT_HTTPGET, 1L);
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, (pm.on_write.has_value()) ? pm.on_write.value() : default_on_write);
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEDATA, &response);

  headers.append("Content-Type: application/json");
  if (pm.user_agent.has_value())
  {
    user_agent = std::format("User-Agent: {}", pm.user_agent.value());
    headers.append(user_agent.c_str());
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_HTTPHEADER, headers.slist);

  CURLcode code = curl_easy_perform(curl_ex.curl);
  if (code != CURLE_OK)
  {
    throw std::runtime_error(curl_easy_strerror(code));
  }

  return from_string< answer_t >(response);
}

template < typename answer_t >
std::future< answer_t >
curlxx::async_get(const params & pm)
{
  return std::async(std::launch::async, &get< answer_t >, std::cref(pm));
}

#endif
