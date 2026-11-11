```cpp run id=account
#include <cassert>
#include <iostream>
#include <stdexcept>

class Account {
public:
    explicit Account(int balance) : balance_(balance) { assert(invariant()); }

    void withdraw(int amount) {
        if (amount <= 0 || amount > balance_) {
            throw std::invalid_argument("cannot withdraw " + std::to_string(amount));
        }
        const int old = balance_;
        balance_ -= amount;
        assert(balance_ == old - amount && "postcondition");
        assert(invariant());
    }

    int balance() const { return balance_; }

private:
    bool invariant() const { return balance_ >= 0; }
    int balance_;
};

static_assert(sizeof(int) >= 4, "int has at least 32 bits");

int main() {
    Account account(100);
    account.withdraw(30);
    std::cout << "balance " << account.balance() << '\n';
    try {
        account.withdraw(500);
    } catch (const std::invalid_argument& e) {
        std::cout << "rejected: " << e.what() << '\n';
    }
}
```

```output
balance 70
rejected: cannot withdraw 500
```

- Verdict: stdlib. C++ takes `assert` from C through `<cassert>`, with the same rule for `NDEBUG`. `static_assert` is a declaration that the compiler checks.
- `assert(condition && "text")` puts a message in the report. A string literal converts to `true`, so it does not change the test.
- In the standard library, a precondition is part of the specification of a function, and a call that violates it has undefined behaviour. The library does not promise to check it.

A failed `assert` writes a report to standard error and calls `abort`.

```cpp fails expect=runtime id=violated
#include <cassert>
#include <iostream>

int average(const int* values, int count) {
    assert(count > 0 && "precondition: no values");
    int sum = 0;
    for (int i = 0; i < count; ++i) {
        sum += values[i];
    }
    return sum / count;
}

int main() {
    int values[] = {2, 4};
    std::cout << average(values, 2) << '\n';
    std::cout << average(values, 0) << '\n';
}
```
