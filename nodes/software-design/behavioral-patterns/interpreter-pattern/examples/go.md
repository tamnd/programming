```go run id=expr
package main

import (
	"fmt"
	"strings"
)

// Expr is the AbstractExpression. Each node type below is one rule.
type Expr interface {
	Eval(env map[string]int) (int, error)
	String() string
}

type Num int
type Var string
type Add struct{ Left, Right Expr }
type Mul struct{ Left, Right Expr }

func (n Num) Eval(map[string]int) (int, error) { return int(n), nil }
func (n Num) String() string                   { return fmt.Sprint(int(n)) }

func (v Var) Eval(env map[string]int) (int, error) {
	value, ok := env[string(v)]
	if !ok {
		return 0, fmt.Errorf("unknown variable %s", string(v))
	}
	return value, nil
}
func (v Var) String() string { return string(v) }

func (a Add) Eval(env map[string]int) (int, error) {
	l, err := a.Left.Eval(env)
	if err != nil {
		return 0, err
	}
	r, err := a.Right.Eval(env)
	if err != nil {
		return 0, err
	}
	return l + r, nil
}
func (a Add) String() string { return a.Left.String() + " + " + a.Right.String() }

func (m Mul) Eval(env map[string]int) (int, error) {
	l, err := m.Left.Eval(env)
	if err != nil {
		return 0, err
	}
	r, err := m.Right.Eval(env)
	if err != nil {
		return 0, err
	}
	return l * r, nil
}
func (m Mul) String() string { return wrap(m.Left) + " * " + wrap(m.Right) }

// wrap puts a sum inside a product in parentheses.
func wrap(e Expr) string {
	if _, ok := e.(Add); ok {
		return "(" + e.String() + ")"
	}
	return e.String()
}

type binding struct {
	name  string
	value int
}

func report(e Expr, bindings ...binding) {
	env := map[string]int{}
	var label []string
	for _, b := range bindings {
		env[b.name] = b.value
		label = append(label, fmt.Sprintf("%s=%d", b.name, b.value))
	}
	value, err := e.Eval(env)
	if err != nil {
		fmt.Printf("%s: error: %v\n", strings.Join(label, " "), err)
		return
	}
	fmt.Printf("%s: %d\n", strings.Join(label, " "), value)
}

func main() {
	e := Mul{Var("x"), Add{Var("y"), Num(2)}}
	fmt.Println("expression:", e)
	report(e, binding{"x", 3}, binding{"y", 4})
	report(e, binding{"x", 5}, binding{"y", 1})

	f := Add{Var("z"), Num(1)}
	fmt.Println("expression:", f)
	report(f, binding{"x", 3}, binding{"y", 4})
}
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. An interface with one method for each operation, and a type for each rule. `Num` and `Var` are named basic types, so a leaf needs no struct.
- `String` is the `fmt.Stringer` method, so `fmt.Println` shows the expression without a call to `show`.
- Each `Eval` returns an error as its second result. The error of the innermost `Var` passes up through every `Add` and `Mul`.
- The bindings are a slice, not a map, because the order of a Go map is not specified, and the label must print in a fixed order.
