```go run id=account
package main

import (
	"errors"
	"fmt"
)

type Account struct {
	balance int
}

var ErrInsufficient = errors.New("insufficient funds")

func (a *Account) invariant() {
	if a.balance < 0 {
		panic(fmt.Sprintf("invariant: balance is %d", a.balance))
	}
}

func (a *Account) Withdraw(amount int) error {
	if amount <= 0 {
		return fmt.Errorf("withdraw %d: amount must be positive", amount)
	}
	if amount > a.balance {
		return fmt.Errorf("withdraw %d from %d: %w", amount, a.balance, ErrInsufficient)
	}
	old := a.balance
	a.balance -= amount
	if a.balance != old-amount {
		panic("postcondition: balance not reduced by amount")
	}
	a.invariant()
	return nil
}

func main() {
	a := &Account{balance: 100}
	fmt.Println(a.Withdraw(30), a.balance)
	err := a.Withdraw(500)
	fmt.Println(err, errors.Is(err, ErrInsufficient), a.balance)
}
```

```output
<nil> 70
withdraw 500 from 70: insufficient funds true 70
```

- Verdict: absent. Go has no `assert`. The Go FAQ says that programmers use assertions "as a crutch to avoid thinking about proper error handling and reporting".
- A bad argument from a caller is an expected failure, so `Withdraw` returns an error. A broken invariant is a bug in `Account` itself, so the check panics.
- The checks are ordinary `if` statements. They run in every build, and no flag removes them.
