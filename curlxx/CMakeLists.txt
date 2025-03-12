set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (APPLE)
  set(CURL_INCLUDE_DIR /opt/homebrew/opt/curl/include)
  set(CURL_LIBRARY /opt/homebrew/opt/curl/lib/libcurl.dylib)
endif()

find_package(CURL REQUIRED)

add_library(curlxx utils.cpp)

target_include_directories(curlxx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CURL_INCLUDE_DIR})

target_link_libraries(curlxx PRIVATE CURL::libcurl)
