```go run id=types-and-methods
package main

import "fmt"

// Account is a struct type; its methods are declared outside the type.
type Account struct {
	Owner   string
	balance int // lower case: visible only inside the package
}

var opened int // package-level state plays the role of a static field

func NewAccount(owner string, balance int) *Account {
	opened++
	return &Account{Owner: owner, balance: balance}
}

// A pointer receiver: the method can change the account.
func (a *Account) Deposit(amount int) { a.balance += amount }

// A value receiver: the method gets a copy.
func (a Account) String() string { return fmt.Sprintf("Account(%s, %d)", a.Owner, a.balance) }

// Methods can belong to any named type, not only to structs.
type Celsius float64

func (c Celsius) Fahrenheit() float64 { return float64(c)*9/5 + 32 }

func main() {
	a := NewAccount("ana", 100)
	b := NewAccount("bo", 0)
	a.Deposit(50)
	fmt.Println(a, b, opened)
	fmt.Println(Celsius(100).Fahrenheit())
	fmt.Printf("%T\n", a)
}
```

```output for=types-and-methods
Account(ana, 150) Account(bo, 0) 2
212
*main.Account
```

```go wrong id=value-receiver
package main

import "fmt"

type Account struct{ balance int }

// A value receiver: Deposit changes a copy of the account.
func (a Account) Deposit(amount int) { a.balance += amount }

func main() {
	a := Account{balance: 100}
	a.Deposit(50)
	fmt.Println(a.balance)
}
```

```output for=value-receiver
100
```

- Verdict: idiom. Go has no classes. A named type, usually a struct, holds the data, and methods are declared separately with a receiver. A method can belong to any named type in the same package, such as `Celsius`, which is a `float64`. Names that start with a capital letter are exported from the package, and the others are private to it, so the package is the unit of encapsulation, not the type. A package-level variable plays the role of a static field, and a function named `NewAccount` plays the role of a constructor.
- The wrong block declares `Deposit` with a value receiver. The method gets a copy of the account and changes the copy, so the balance stays 100. Use a pointer receiver, `func (a *Account)`, for methods that change the receiver, and use the same kind of receiver for all methods of a type.
