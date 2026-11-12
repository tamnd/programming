```go run id=five-levels
package main

import "fmt"

func main() {
	// Go has five levels of binary precedence. & and << are at the level of *,
	// so these mean (x & 1) == 0 and (1 << 2) + 1.
	x := 6
	fmt.Println(x&1 == 0, 1<<2+1)

	// In C the same text means x & (1 == 0) and 1 << (2 + 1).
	fmt.Println(x&(1-1), 1<<(2+1))

	// && binds more tightly than ||.
	t, f := true, false
	fmt.Println(t || f && f, (t || f) && f)

	// ++ and -- are statements, not operators in expressions, so i++ + 1 does not compile.
	i := 1
	i++
	fmt.Println(i)

	// gofmt shows the grouping with spaces: it removes the spaces around the operators that bind more tightly.
	a, b, c := 2, 3, 4
	y := a + b*c
	z := a*b + c
	fmt.Println(y, z)
}
```

```output for=five-levels
true 5
0 8
true false
2
14 10
```

- Verdict: native. Go has five levels of binary operators, and `&`, `&^`, `<<` and `>>` are at the level of `*`. So `x&1 == 0` is `(x & 1) == 0`, and `1<<2+1` is 5, not 8 as in C. `&&` binds more tightly than `||`. `++` is a statement, not an operator in an expression. `gofmt` removes the spaces around the operators that bind more tightly, so `a + b*c` shows its own grouping.
