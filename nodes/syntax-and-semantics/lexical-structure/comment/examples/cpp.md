```cpp run id=raw-strings-and-if-0
#include <iostream>
#include <string>

int main() {
    // A raw string keeps the comment markers as text.
    std::string s = R"(/* not a comment */ // nor this)";
    std::cout << s << "\n";

#if 0
    This block is not compiled. It can hold code that does not parse,
    and it can hold /* a block comment */ without a nesting problem.
#endif

    // The preprocessor removes comments before it expands macros,
    // so a macro body cannot make a comment.
#define SLASH /
#define STR(x) #x
#define XSTR(x) STR(x)
    std::cout << XSTR(SLASH SLASH x) << "\n"; // two / tokens, not a comment

    /** A Doxygen comment is a normal comment to the compiler. */
    std::cout << "done\n";
}
```

```output for=raw-strings-and-if-0
/* not a comment */ // nor this
/ / x
done
```

- Verdict: native. C++ has the comments of C. A raw string keeps comment markers as text. `#if 0` removes a block that can contain block comments. Comments are removed before macro expansion, so the macro `SLASH` gives the token `/`, and two of them stay two tokens, as `/ / x` shows. Doxygen reads `/** */` comments, but to the compiler they are normal comments.
