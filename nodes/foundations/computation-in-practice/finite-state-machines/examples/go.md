```go run id=iota-constants-and-switch
package main

import (
	"fmt"
	"regexp"
)

type state int

const (
	start state = iota
	sign
	integer
	dot
	frac
	dead
)

func step(s state, ch rune) state {
	digit := ch >= '0' && ch <= '9'
	switch {
	case s == start && (ch == '+' || ch == '-'):
		return sign
	case (s == start || s == sign || s == integer) && digit:
		return integer
	case s == integer && ch == '.':
		return dot
	case (s == dot || s == frac) && digit:
		return frac
	}
	return dead
}

func accepts(text string) bool {
	s := start
	for _, ch := range text {
		s = step(s, ch)
	}
	return s == integer || s == frac
}

func main() {
	inputs := []string{"42", "-3.5", "1.", "+", "007", ".5", "4.2.1"}
	for _, t := range inputs {
		fmt.Println(t, accepts(t))
	}
	re := regexp.MustCompile(`^[+-]?[0-9]+(\.[0-9]+)?$`)
	agree := true
	for _, t := range inputs {
		agree = agree && re.MatchString(t) == accepts(t)
	}
	fmt.Println("regex agrees:", agree)
}
```

```output for=iota-constants-and-switch
42 true
-3.5 true
1. false
+ false
007 true
.5 false
4.2.1 false
regex agrees: true
```

- Verdict: idiom. Go declares the states as a named integer type with `iota` constants, and a `switch` with no tag tests one condition for each transition. The `regexp` package compiles regular expressions to automata and runs in time linear in the length of the input.
