```go norun reason=fragment group=account id=account file=account.go
package account

import "fmt"

// InsufficientFundsError is the error of a withdrawal larger than the balance.
type InsufficientFundsError struct {
	Balance, Amount int
}

func (e *InsufficientFundsError) Error() string {
	return fmt.Sprintf("balance %d, asked %d", e.Balance, e.Amount)
}

// Account holds a balance in whole units.
type Account struct {
	Balance int
}

func (a *Account) Deposit(amount int) {
	a.Balance += amount
}

func (a *Account) Withdraw(amount int) error {
	if amount > a.Balance {
		return &InsufficientFundsError{a.Balance, amount}
	}
	a.Balance -= amount
	return nil
}
```

```go test group=account id=account-test file=account_test.go
package account

import (
	"errors"
	"fmt"
	"testing"
)

// newAccount is the fixture. Each test calls it to get its own account.
func newAccount() *Account {
	return &Account{Balance: 100}
}

func TestDeposit(t *testing.T) {
	a := newAccount()
	a.Deposit(50)
	if a.Balance != 150 {
		t.Errorf("balance = %d, want 150", a.Balance)
	}
}

func TestWithdraw(t *testing.T) {
	tests := []struct{ amount, rest int }{
		{30, 70},
		{100, 0},
	}
	for _, tt := range tests {
		t.Run(fmt.Sprintf("withdraw %d", tt.amount), func(t *testing.T) {
			a := newAccount()
			if err := a.Withdraw(tt.amount); err != nil {
				t.Fatalf("Withdraw(%d) returned error %v", tt.amount, err)
			}
			if a.Balance != tt.rest {
				t.Errorf("balance = %d, want %d", a.Balance, tt.rest)
			}
		})
	}
}

func TestWithdrawMoreThanBalance(t *testing.T) {
	a := newAccount()
	var e *InsufficientFundsError
	if err := a.Withdraw(150); !errors.As(err, &e) {
		t.Fatalf("Withdraw(150) returned %v, want an InsufficientFundsError", err)
	}
	if a.Balance != 100 {
		t.Errorf("balance = %d, want 100", a.Balance)
	}
}
```

```output
=== RUN   TestDeposit
--- PASS: TestDeposit ($TIME)
=== RUN   TestWithdraw
=== RUN   TestWithdraw/withdraw_30
=== RUN   TestWithdraw/withdraw_100
--- PASS: TestWithdraw ($TIME)
    --- PASS: TestWithdraw/withdraw_30 ($TIME)
    --- PASS: TestWithdraw/withdraw_100 ($TIME)
=== RUN   TestWithdrawMoreThanBalance
--- PASS: TestWithdrawMoreThanBalance ($TIME)
PASS
ok  	example	$TIME
```

- Verdict: stdlib. The `testing` package and the `go test` command come with Go. `go test` finds the functions `TestXxx(t *testing.T)` in the files whose names end in `_test.go`, builds a test binary and runs it.
- Go has no fixture hook for each test. A test calls a helper function, such as `newAccount`, to get a new value. `TestMain` can do setup once for the whole package.
- `t.Run` makes a subtest for each row of the table. The report gives each subtest its own name, and `go test -run 'TestWithdraw/withdraw_30'` runs one row.
- Go has no assertion functions in the standard library. A test compares values with `if` and calls `t.Errorf` or `t.Fatalf`.
