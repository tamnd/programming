```go run id=move-to-the-data
package main

import "fmt"

type Account struct {
	Balance        int
	OverdraftLimit int
	Frozen         bool
}

// After: the rule is a method on the type whose data it reads.
func (a Account) CanWithdraw(amount int) bool {
	return !a.Frozen && a.Balance+a.OverdraftLimit >= amount
}

// Before: the cash machine package reads the fields of the account.
func canWithdrawBefore(a Account, amount int) bool {
	if a.Frozen {
		return false
	}
	return a.Balance+a.OverdraftLimit >= amount
}

func main() {
	cases := []struct {
		account  Account
		amount   int
		expected bool
	}{
		{Account{100, 50, false}, 150, true},
		{Account{100, 50, false}, 151, false},
		{Account{100, 0, true}, 10, false},
	}
	for _, c := range cases {
		if canWithdrawBefore(c.account, c.amount) != c.expected {
			panic(fmt.Sprintf("before %d", c.amount))
		}
		if c.account.CanWithdraw(c.amount) != c.expected {
			panic(fmt.Sprintf("after %d", c.amount))
		}
	}
	fmt.Println("CanWithdraw: tests pass")
}
```

```output
CanWithdraw: tests pass
```

- In Go, a method must be declared in the same package as its type. Moving the function to a method also moves it to the package of `Account`.
- The fields start with a capital letter, so other packages can read them. After the move, they can be renamed to `balance` and `overdraftLimit`, which hides them outside the package.
