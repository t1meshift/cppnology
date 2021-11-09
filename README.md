# cppnology

## Description

A library written in C++17 to utilize Synology NAS WebAPI.

It uses a ton of dependencies:

- [refl-cpp](https://github.com/veselink1/refl-cpp) for static reflection;
- [magic_enum](https://github.com/Neargye/magic_enum) to parse enums in [the serializer](include/cppnology/CprSerializer.h);
- [C++ Requests](https://github.com/libcpr/cpr) to make requests to the API;
- [fmtlib](https://github.com/fmtlib/fmt) for string formatting;
- [nlohmann::json](https://github.com/nlohmann/json) for JSON support;
- [CxxUrl](https://github.com/chmike/CxxUrl) for proper URL building.

## Features

The library has QuickConnect support.

As for now, these API methods have been implemented:

- `SYNO.API.Info`
    - query
- `SYNO.API.Auth`:
    - login (v6)
        - *no OTP support*

Upcoming features:
- [ ] Doxygen documentation
- [ ] Wildcard method headers (e.g. to include all methods of SYNO.API.*)
- [ ] Download Station API
- [ ] File Station API
- [ ] `SYNO.Entry.Request`
- [ ] `SYNO.API.Encryption`
- [ ] `SYNO.API.Auth` with encrypted credentials support

## Usage

```cpp
#include <cppnology/Instance.h>
#include <cppnology/API/Info/query.h>

namespace Syno = cppnology;

// to connect with QuickConnect
auto a = Syno::Instance(Syno::QuickConnect("quickconnect_id"), "login", "password");

// to connect with URL
auto b = Syno::Instance(Url().scheme("http").host("127.0.0.1").port(5000), "login", "password");

a.call(Syno::API::Info::query<1>{}); // call SYNO.API.Info with method "query"

// call SYNO.API.Info with method "query" with param "query" set to "SYNO.FileStation."
b.call(Syno::API::Info::query<1>{"SYNO.FileStation."});
```

## License

cppnology is distributed under the [MIT license](LICENSE).
