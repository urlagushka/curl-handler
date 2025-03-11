set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL REQUIRED)

add_library(curlxx handler.cpp utils.cpp)

target_include_directories(curlxx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(curlxx PRIVATE CURL::libcurl)
