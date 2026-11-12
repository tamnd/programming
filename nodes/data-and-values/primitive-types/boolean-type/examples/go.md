```go run id=parse-and-set
package main

import (
	"fmt"
	"strconv"
)

func main() {
	// bool is its own type: if needs a bool, and there is no conversion from int.
	x := 5
	nonZero := x != 0
	fmt.Println(nonZero, x > 3 && x < 10, !nonZero)

	// The zero value is false.
	var ready bool
	fmt.Println(ready)

	// strconv parses 1, t, T, TRUE, true, True and the false forms.
	for _, s := range []string{"true", "T", "1", "yes"} {
		b, err := strconv.ParseBool(s)
		fmt.Println(s, b, err != nil)
	}
	fmt.Println(strconv.FormatBool(true))

	// A map to bool is a set: a missing key gives false.
	seen := map[string]bool{}
	for _, w := range []string{"a", "b", "a"} {
		if seen[w] {
			fmt.Println("duplicate", w)
		}
		seen[w] = true
	}

	// Go has no conversion from bool to int, so it needs an if.
	count := 0
	for _, ok := range []bool{true, false, true} {
		if ok {
			count++
		}
	}
	fmt.Println(count)
}
```

```output for=parse-and-set
true true false
false
true true false
T true false
1 true false
yes false true
true
duplicate a
2
```

- Verdict: native. A condition must be a `bool`, and the zero value is `false`. `strconv.ParseBool` accepts `"true"`, `"T"` and `"1"` and returns an error for `"yes"`. A `map[string]bool` works as a set, because a missing key gives `false`. Go has no conversion from `bool` to `int`, so counting the true values needs an `if`.
