```go run id=interfaces
package main

import "fmt"

// Cases are types; the interface fixes one operation, Eval.
type Expr interface{ Eval() int }

type Num struct{ V int }
type Add struct{ L, R Expr }

func (n Num) Eval() int { return n.V }
func (a Add) Eval() int { return a.L.Eval() + a.R.Eval() }

// A new case: a new type with an Eval method.
type Neg struct{ X Expr }

func (n Neg) Eval() int { return -n.X.Eval() }

// A new case can bring its own Show; Show asks for it first.
type Shower interface{ Show() string }

// A new operation: a function with a type switch over the cases.
func Show(e Expr) string {
	if s, ok := e.(Shower); ok {
		return s.Show()
	}
	switch e := e.(type) {
	case Num:
		return fmt.Sprint(e.V)
	case Add:
		return "(" + Show(e.L) + " + " + Show(e.R) + ")"
	case Neg:
		return "-" + Show(e.X)
	}
	return fmt.Sprintf("<%T?>", e)
}

// A case added after Show was written.
type Mul struct{ L, R Expr }

func (m Mul) Eval() int    { return m.L.Eval() * m.R.Eval() }
func (m Mul) Show() string { return "(" + Show(m.L) + " * " + Show(m.R) + ")" }

func main() {
	e := Add{Num{1}, Neg{Add{Num{2}, Num{3}}}}
	fmt.Println(Show(e), "=", e.Eval())
	m := Add{Num{1}, Mul{Num{2}, Num{3}}}
	fmt.Println(Show(m), "=", m.Eval())
}
```

```output for=interfaces
(1 + -(2 + 3)) = -4
(1 + (2 * 3)) = 7
```

```go fails expect=compile id=foreign-method
package main

import (
	"fmt"
	"time"
)

// Add an operation to a type from another package.
func (d time.Duration) Minutes2() float64 { return d.Minutes() }

func main() {
	fmt.Println(time.Duration(90 * time.Second).Minutes2())
}
```

- Verdict: idiom. An interface fixes a set of methods, and any type in any package that has them is a case, with no declaration. A new operation is a function with a type switch over the known cases. A new case can bring its own version of the operation by implementing an optional interface, here `Shower`, which the function checks first. The type switch has no exhaustiveness check, so a forgotten case reaches the fallback at run time.
- The failing block defines a method on `time.Duration`. Go allows methods only on types that the same package defines, so code cannot add an operation as a method of a type from another package. Wrap the type in a new named type, or write a function.
