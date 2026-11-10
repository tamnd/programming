```go run id=conversions
package main

import (
	"fmt"
	"strconv"
)

func main() {
	var small int32 = 300
	wide := int64(small)
	low := byte(small)
	f := 3.99
	fmt.Println(wide, low, int(f), int(-f))

	n, err := strconv.Atoi("42")
	fmt.Println(n+1, err)
	_, err = strconv.Atoi("42abc")
	fmt.Println(err)

	var x any = "text"
	if s, ok := x.(string); ok {
		fmt.Println("string of length", len(s))
	}
	_, ok := x.(int)
	fmt.Println("int?", ok)
	fmt.Println(strconv.Itoa(65), string(rune(65)))
}
```

```output for=conversions
300 44 3 -3
43 <nil>
strconv.Atoi: parsing "42abc": invalid syntax
string of length 4
int? false
65 A
```

- Verdict: native. Go has no implicit conversion between numeric types. Even `int32` to `int64` needs `int64(x)`.
- An integer conversion keeps the low bits, so `byte(300)` from a variable is 44. A constant that does not fit is a compile error.
- `strconv` parses and formats text, and returns an error value. `x.(T)` is a type assertion on an interface value; the two-value form reports failure instead of a panic.
- `string(rune(65))` is `"A"`, a code point, not the digits `"65"`.
