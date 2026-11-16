```cpp run id=function-signature-cpp
#include <iostream>

double average(int a, int b);

int main() {
    std::cout << average(3, 4) << "\n";
}

double average(int a, int b) {
    return (a + b) / 2.0;
}
```

```output for=function-signature-cpp
3.5
```

- Verdict: native. The declaration before `main` gives the signature, and the definition after `main` gives the body.
