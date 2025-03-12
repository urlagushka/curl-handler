# curlxx
Async cURL for C++20
| Method | Status | Application/Type |
| --- | --- | --- |
| GET | Testing | json |
| POST | Testing | json |
| PUT | - | - |
| DELETE | - | - |
| PATCH | - | - |

## Supported methods
#### Request params
```cpp
curlxx::params pm = {
  .url = std::string, // required param
  .query = nlohmann::json{} // optional param | post request only
  .user_agent = std::string, // optional param
  .on_write = curlxx::on_write_sign *, // optional param | by default used default_on_write
  .is_debug = bool // optional param | by default is false
};
```
#### Common
```cpp
curlxx::params pm = { ... };
auto answer = curlxx::METHOD< ANSWER_TYPE >(pm);

// ANSWER_TYPE must be constructable from std::string
// in other way, you can make a template specialization for from_string method
// here is template specialization for nlohmann::json (built-in) (curlxx.hpp)
template < >
nlohmann::json
from_string(std::string_view rhs)
{
  return nlohmann::json::parse(rhs);
}
```
#### Get
```cpp
// simple get
curlxx::params get_pm = {
  .url = "https://catfact.ninja/fact",
  .user_agent = "firefox"
};

auto answer = curlxx::get< nlohmann::json >(get_pm);
std::cout << answer.dump(2) << std::endl;

// async get
curlxx::params async_get_pm = {
  .url = "https://catfact.ninja/fact",
  .user_agent = "firefox"
};

auto answer = curlxx::async_get< nlohmann::json >(async_get_pm);
std::cout << answer.get().dump(2) << std::endl;
```

#### Post
```cpp
// simple post
curlxx::params post_pm = {
  .url = "https://httpbin.org/post",
  .query = nlohmann::json{"post", "test"},
  .user_agent = "firefox"
};

auto answer = curlxx::post< nlohmann::json >(post_pm);
std::cout << answer.dump(2) << std::endl;

// async post
curlxx::params async_post_pm = {
  .url = "https://httpbin.org/post",
  .query = nlohmann::json{"async", "post", "test"},
  .user_agent = "firefox"
};

auto answer = curlxx::async_post< nlohmann::json >(async_post_pm);
std::cout << answer.get().dump(2) << std::endl;
```

## Example build Linux (required OpenSSl in CURL):
```sh
mkdir build && cd build
cmake ..
make
./curlxx_example
```

## Example build MacOS (required OpenSSL in CURL)
```sh
brew uninstall curl
brew install curl-openssl

export LDFLAGS="-L/opt/homebrew/opt/curl/lib"
export CPPFLAGS="-I/opt/homebrew/opt/curl/include"
mkdir build && cd build
cmake ..
make
./curlxx_example
```