#ifndef UTILS_HPP
#define UTILS_HPP

#include <curl/curl.h>

namespace curlxx::utils
{
  struct curl_fd
  {
    CURL * curl = nullptr;
    
    curl_fd();
    ~curl_fd();
  };

  struct slist_fd
  {
    curl_slist * slist = nullptr;

    ~slist_fd();
    void append(const char * header);
  };
}

curlxx::utils::curl_fd::curl_fd():
  curl(curl_easy_init())
{
  if (curl == nullptr)
  {
    throw std::runtime_error("curl init failed!");
  }
}

curlxx::utils::curl_fd::~curl_fd()
{
  if (curl)
  {
    curl_easy_cleanup(curl);
  }
}

curlxx::utils::slist_fd::~slist_fd()
{
  curl_slist_free_all(slist);
}

void curlxx::utils::slist_fd::append(const char * header)
{
  curl_slist * tmp = curl_slist_append(slist, header);

  if (tmp == nullptr)
  {
    throw std::runtime_error("slist append error!");
  }
  slist = tmp;
}

#endif
