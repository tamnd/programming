```go run id=embedding
package main

import "fmt"

type Account struct {
	Owner   string
	balance int
}

func (a *Account) Deposit(n int)    { a.balance += n }
func (a *Account) Balance() int     { return a.balance }
func (a *Account) Describe() string { return fmt.Sprintf("account of %s: %d", a.Owner, a.balance) }

type Checking struct {
	Account   // embedded: its fields and methods are promoted to Checking
	Overdraft int
}

// Describe on Checking shadows the promoted method and calls it by name.
func (c *Checking) Describe() string {
	return fmt.Sprintf("%s (overdraft %d)", c.Account.Describe(), c.Overdraft)
}

type Describer interface{ Describe() string }

func main() {
	c := &Checking{Account: Account{Owner: "bo"}, Overdraft: 100}
	c.Deposit(50) // promoted from Account
	fmt.Println(c.Owner, c.Balance())
	for _, d := range []Describer{&c.Account, c} {
		fmt.Println(d.Describe())
	}
}
```

```output for=embedding
bo 50
account of bo: 50
account of bo: 50 (overdraft 100)
```

```go fails expect=compile id=not-a-subtype
package main

type Account struct{ Owner string }

type Checking struct {
	Account
	Overdraft int
}

func main() {
	c := &Checking{Account{"bo"}, 100}
	var a *Account = c // a *Checking is not a *Account
	_ = a
}
```

- Verdict: absent. Go has no inheritance. A struct can embed another type, and the fields and methods of the embedded type are promoted, so `c.Deposit(50)` and `c.Owner` work on a `Checking`. A method of the outer type with the same name shadows the promoted one, and it calls the inner one by its full name, `c.Account.Describe()`.
- Embedding is composition, not subtyping. The failing block assigns a `*Checking` to a `*Account`, which the compiler rejects, because the types are not related. Code that must accept both takes an interface, such as `Describer`, which both types satisfy.
