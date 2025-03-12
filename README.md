# curlxx
## NOT READY | NEED TEST | MEMORY LEAKS
async curl handler for cpp 20
#### now works only with application/json - need rework

## Supported methods
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

## Example build:
```sh
mkdir build && cd build
cmake ..
make
./curlxx_example
```
