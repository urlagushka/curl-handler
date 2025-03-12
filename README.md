# curlxx
## NOT READY | NEED TEST | MEMORY LEAKS
async curl handler for cpp 20
#### now works only with application/json - need rework

## Supported methods
#### Get
```cpp
// simple get
auto answer = qq.get< nlohmann::json >("https://catfact.ninja/fact");
std::cout << answer.dump(2) << std::endl;

// async get
auto answer = qq.async_get< nlohmann::json >("https://catfact.ninja/fact");
std::cout << answer.get().dump(2) << std::endl;
```

#### Post
```cpp
// simple post
auto answer = qq.post< nlohmann::json >("https://httpbin.org/post", {"post", "test"});
std::cout << answer.dump(2) << std::endl;

// async post
auto answer = qq.async_post< nlohmann::json >("https://httpbin.org/post", {"async", "post", "test"});
std::cout << answer.get().dump(2) << std::endl;
```

## Example build:
```sh
mkdir build && cd build
cmake ..
make
./curlxx_example
```
