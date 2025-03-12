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

#endif
