#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <string>
#include <utility>
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
  using on_write_sign = std::size_t (*)(char * , std::size_t, std::size_t, std::string &);

  struct params
  {
    std::optional< std::string > url;
    std::optional< nlohmann::json > query;
    std::optional< std::string > user_agent;

    std::optional< on_write_sign > on_write;
    std::optional< bool > is_debug;
  };

  template < correct_answer_t answer_t >
  answer_t
  post(const params & pm);

  template < correct_answer_t answer_t >
  std::future< answer_t >
  async_post(const params & pm);

  template < correct_answer_t answer_t >
  answer_t
  get(const params & pm);

  template < correct_answer_t answer_t >
  std::future< answer_t >
  async_get(const params & pm);
}

template < correct_answer_t answer_t >
answer_t
curlxx::post(const params & pm)
{
  if (!pm.url.has_value())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd curl_ex;
  std::string response;
  std::string user_agent;

  if (pm.is_debug)
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_URL, pm.url.value().c_str());
  curl_easy_setopt(curl_ex.curl, CURLOPT_POST, 1L);
  if (pm.query.has_value())
  {
    const char * tmp = pm.query.value().dump().c_str();
    curl_easy_setopt(curl_ex.curl, CURLOPT_POSTFIELDS, tmp);
    curl_easy_setopt(curl_ex.curl, CURLOPT_POSTFIELDSIZE, sizeof(tmp) / sizeof(char));
  }

  if (pm.on_write.has_value())
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, pm.on_write.value());
  }
  else
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, default_on_write);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEDATA, &response);

  utils::slist_fd headers;
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
    throw std::runtime_error(std::format("POST {} ERROR\n{}", pm.url.value(), curl_easy_strerror(code)));
  }

  return nlohmann::json::parse(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curlxx::async_post(const params & pm)
{
  return std::async(std::launch::async, &post< answer_t >, pm);
}

template < correct_answer_t answer_t >
answer_t
curlxx::get(const params & pm)
{
  if (!pm.url.has_value())
  {
    throw std::runtime_error("url is null!");
  }

  utils::curl_fd curl_ex;
  std::string response;
  std::string user_agent;

  if (pm.is_debug)
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_VERBOSE, 1L);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_URL, pm.url.value().c_str());
  curl_easy_setopt(curl_ex.curl, CURLOPT_HTTPGET, 1L);

  if (pm.on_write.has_value())
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, pm.on_write.value());
  }
  else
  {
    curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEFUNCTION, default_on_write);
  }
  curl_easy_setopt(curl_ex.curl, CURLOPT_WRITEDATA, &response);

  utils::slist_fd headers;
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
    throw std::runtime_error(std::format("GET {} ERROR\n{}", pm.url.value(), curl_easy_strerror(code)));
  }

  return nlohmann::json::parse(response);
}

template < correct_answer_t answer_t >
std::future< answer_t >
curlxx::async_get(const params & pm)
{
  return std::async(std::launch::async, &get< answer_t >, pm);
}

#endif
