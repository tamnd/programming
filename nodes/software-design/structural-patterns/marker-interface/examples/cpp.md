```cpp run id=redacting-logger
#include <iostream>
#include <string>
#include <type_traits>

struct Username { std::string value; };
struct Password { std::string value; };
struct ApiKey { std::string value; };

// The marker is a trait outside the classes.
template <class T> struct is_sensitive : std::false_type {};
template <> struct is_sensitive<Password> : std::true_type {};
template <> struct is_sensitive<ApiKey> : std::true_type {};

template <class T>
void log(const std::string& label, const T& item) {
    if constexpr (is_sensitive<T>::value) {
        std::cout << label << ": [redacted]\n";
    } else {
        std::cout << label << ": " << item.value << "\n";
    }
}

int main() {
    log("user", Username{"ann"});
    log("password", Password{"hunter2"});
    log("api key", ApiKey{"k-123"});
}
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. A type trait marks a type from outside its class, so a library type can be marked too. `if constexpr` tests the marker at compile time, and the unmarked branch is not even compiled for a marked type.
- An empty base class with `std::is_base_of_v` is the form closest to the Java interface.
