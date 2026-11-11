```cpp run id=lambda-and-lvalue-expressions
#include <iostream>
#include <string>

int main() {
    int score = 70;
    const std::string label = [&] { // an immediately invoked lambda
        if (score >= 90) return "A";
        if (score >= 50) return "pass";
        return "fail";
    }(); // lets a const variable take its value from statements
    std::cout << label << '\n';

    int a = 1, b = 2;
    (a < b ? a : b) = 0; // a conditional of two variables is a variable
    std::cout << a << ' ' << b << '\n';

    int x = 3;
    int& r = (x += 2); // compound assignment gives the variable itself
    r *= 10;
    std::cout << x << '\n';
}
```

```output for=lambda-and-lvalue-expressions
pass
0 2
50
```

- Verdict: native. C++ has the expressions of C, and some of them are lvalues, which name an object: a conditional of two variables of the same type is itself a variable, and compound assignment returns the variable it changed. A lambda that is called at once turns a block of statements into an expression, which lets a `const` variable take its value from `if` statements.
