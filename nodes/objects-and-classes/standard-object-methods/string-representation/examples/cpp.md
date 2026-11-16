```cpp run id=stream-insertion-operator
#include <iostream>
#include <sstream>
#include <string>

struct Book {
    std::string title;
    int year;
};

// The stream operator gives the display form to std::cout and string streams.
std::ostream& operator<<(std::ostream& os, const Book& b) {
    return os << b.title << " (" << b.year << ")";
}

int main() {
    Book b{"Dune", 1965};
    std::cout << b << '\n';
    std::ostringstream out;
    out << "Read " << b;
    std::string s = out.str();
    std::cout << s << ' ' << s.size() << '\n';
}
```

```output for=stream-insertion-operator
Dune (1965)
Read Dune (1965) 16
```

- Verdict: idiom. C++ has no string method on objects. The convention is an `operator<<` for `std::ostream`, which `std::cout` and `std::ostringstream` use.
- A string stream turns the display form into a `std::string`, for code that needs a string and not a stream.
- `std::format` and `std::print` use a different hook, a specialization of `std::formatter` for the type; a type that has only `operator<<` does not work with them.
