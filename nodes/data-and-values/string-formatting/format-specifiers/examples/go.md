```go run id=fmt-verbs
package main

import "fmt"

type point struct{ X, Y int }

func main() {
	x := 3.14159
	fmt.Printf("[%8.2f] [%-8.2f] [%08.2f] [%+.1f]\n", x, x, x, x)
	fmt.Printf("[%6s] [%-6s] [%q] [%x] [%#o]\n", "abc", "abc", "abc", 255, 8)
	p := point{1, 2}
	fmt.Printf("%v %+v %#v %T\n", p, p, p, p)
	fmt.Printf("%[2]s %[1]s %[2]s\n", "a", "b")
}
```

```output for=fmt-verbs
[    3.14] [3.14    ] [00003.14] [+3.1]
[   abc] [abc   ] ["abc"] [ff] [010]
{1 2} {X:1 Y:2} main.point{X:1, Y:2} main.point
b a b
```

- Verdict: stdlib. The package `fmt` has the flags, widths and precisions of C, and verbs of its own: `%v` for the default format of any value, `%+v` with field names, `%#v` in Go syntax, `%T` for the type and `%q` for a quoted string. `%[2]s` picks an argument by position.
