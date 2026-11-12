```go run id=short-declarations-and-shadowing
package main

import (
	"errors"
	"fmt"
	"strconv"
)

type counter struct{ n int }

func (c *counter) inc() int { c.n++; return c.n }

func parse(s string) (int, error) {
	n, err := strconv.Atoi(s)
	if err != nil {
		return 0, errors.New("bad number " + strconv.Quote(s))
	}
	return n, nil
}

func main() {
	// := declares new names; with at least one new name on the left, it rebinds the others.
	a, err := parse("12")
	b, err := parse("x")
	fmt.Println(a, b, err)

	// := in an inner block makes a new x that shadows the outer x.
	x := 1
	if a > 0 {
		x := 2
		x++
	}
	fmt.Println(x)

	// Each iteration of a for loop has its own loop variable.
	var fs []func() int
	for i := range 3 {
		fs = append(fs, func() int { return i })
	}
	fmt.Println(fs[0](), fs[1](), fs[2]())

	// A method value binds the receiver when it is made.
	c := &counter{}
	next := c.inc
	next()
	next()
	fmt.Println(c.n)
}
```

```output for=short-declarations-and-shadowing
12 0 bad number "x"
1
0 1 2
2
```

- Verdict: native. `:=` declares new names, and it can redeclare a name in the same block when at least one name on the left is new. A `:=` in an inner block declares a new variable that shadows the outer one, so the outer `x` stays `1`. Each iteration of a `for` loop has its own loop variable. A method value binds the receiver when it is made.
