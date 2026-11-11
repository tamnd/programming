```go run id=unexported-fields
package main

import (
	"errors"
	"fmt"
	"slices"
)

// Account is exported; its fields are not, so other packages
// can change them only through the methods.
type Account struct {
	balance int64
	history []int64
}

func NewAccount() *Account { return &Account{} }

func (a *Account) Deposit(amount int64) error {
	if amount <= 0 {
		return errors.New("deposit must be positive")
	}
	a.history = append(a.history, amount)
	a.balance += amount
	return nil
}

func (a *Account) Withdraw(amount int64) error {
	if amount > a.balance {
		return errors.New("insufficient funds")
	}
	a.history = append(a.history, -amount)
	a.balance -= amount
	return nil
}

func (a *Account) Balance() int64 { return a.balance }

// History returns a copy, so callers cannot change the account's slice.
func (a *Account) History() []int64 { return slices.Clone(a.history) }

func main() {
	a := NewAccount()
	a.Deposit(100)
	a.Withdraw(30)
	h := a.History()
	h[0] = 1_000_000
	fmt.Println(a.Balance(), a.History(), a.Withdraw(500))
}
```

```output for=unexported-fields
70 [100 -30] insufficient funds
```

```go wrong id=shared-slice
package main

import "fmt"

type Stack struct {
	items []int
}

func (s *Stack) Push(x int) { s.items = append(s.items, x) }

// Items returns the internal slice: the caller shares its backing array.
func (s *Stack) Items() []int { return s.items }

func main() {
	var s Stack
	s.Push(1)
	s.Push(2)
	view := s.Items()
	view[0] = 99 // writes into the stack's own array
	fmt.Println(s.Items())
}
```

```output for=shared-slice
[99 2]
```

- Verdict: native. Go controls access by package, not by type. A name that starts with an upper case letter is exported from its package, and every other name is visible only inside the package. A type usually has unexported fields, a constructor function such as `NewAccount`, and exported methods. Inside one package, all code can read and write the fields, so the example, which is one file in package `main`, shows the shape of the code but not the check, which applies only to other packages.
- The wrong block returns the internal slice. A slice is a view of an array, so the caller's `view` and the stack share one array, and the write to `view[0]` changes the stack. Return `slices.Clone(s.items)`, as `History` in the run block does.
