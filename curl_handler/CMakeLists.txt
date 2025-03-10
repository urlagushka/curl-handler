set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(curl_handler curl_handler.cpp)

target_include_directories(curl_handler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)