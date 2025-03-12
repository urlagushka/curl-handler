# curlxx
## NOT READY | NEED TEST | MEMORY LEAKS (MACOS ONLY, LINUX OK)
async curl handler for cpp 20
#### now works only with application/json - need rework

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
