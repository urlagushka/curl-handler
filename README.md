# curl-handler
async curl handler for cpp 20


MacOS build:
I have Apple clang 16, that doesn't support ```std::jthread```, if you have the same, you need install gcc through brew
```sh
brew install gcc
gcc-14 --version
```

Compilation on MacOS using gcc:
```
mkdir build && cd build
cmake -DCMAKE_C_COMPILER=/opt/homebrew/bin/gcc-14 -DCMAKE_CXX_COMPILER=/opt/homebrew/bin/g++-14 ..
```
