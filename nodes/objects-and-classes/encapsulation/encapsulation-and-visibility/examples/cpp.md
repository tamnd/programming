```cpp run id=private-members
#include <iostream>
#include <stdexcept>
#include <vector>

class Account {
public:
    void deposit(long amount)
    {
        if (amount <= 0) throw std::invalid_argument("deposit must be positive");
        history_.push_back(amount);
        balance_ += amount;
    }

    void withdraw(long amount)
    {
        if (amount > balance_) throw std::runtime_error("insufficient funds");
        history_.push_back(-amount);
        balance_ -= amount;
    }

    long balance() const { return balance_; }
    const std::vector<long>& history() const { return history_; }  // read-only view

    bool same_balance(const Account& other) const
    {
        return balance_ == other.balance_;  // private is per class, not per object
    }

private:
    long balance_ = 0;
    std::vector<long> history_;

    friend void audit(const Account&);  // a friend may read the private members
};

void audit(const Account& a) { std::cout << "audit " << a.balance_ << '\n'; }

int main()
{
    Account a, b;
    a.deposit(100);
    a.withdraw(30);
    b.deposit(70);
    std::cout << a.balance() << ' ' << a.history().size() << ' ' << a.same_balance(b) << '\n';
    try {
        a.withdraw(500);
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }
    audit(a);
}
```

```output for=private-members
70 2 1
insufficient funds
audit 70
```

```cpp fails expect=compile id=private-access
class Account {
public:
    long balance() const { return balance_; }

private:
    long balance_ = 0;
};

int main()
{
    Account a;
    a.balance_ = 1000000;  // 'balance_' is a private member of 'Account'
    return 0;
}
```

- Verdict: native. A class has `public`, `protected` and `private` sections, and members of a `class` are private by default. Access is per class, so `same_balance` reads the balance of another account, and a `friend` declaration gives a function or another class access to the private members. A getter that returns a `const` reference gives a read-only view without a copy. Access control is a compile-time check, not a protection of memory: code with a pointer can still reach the bytes.
- The failing block writes the private member `balance_` from `main`, which the compiler rejects. The pimpl idiom, a pointer to a private implementation class, also hides the layout of the members, so that changes to them do not force callers to recompile.
