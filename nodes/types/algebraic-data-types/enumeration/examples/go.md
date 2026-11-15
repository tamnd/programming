```go run id=light
package main

import "fmt"

type Light int

const (
	Red Light = iota
	Amber
	Green
)

func (l Light) String() string {
	switch l {
	case Red:
		return "red"
	case Amber:
		return "amber"
	case Green:
		return "green"
	}
	return fmt.Sprintf("Light(%d)", int(l))
}

func after(l Light) Light {
	switch l {
	case Red:
		return Green
	case Green:
		return Amber
	default:
		return Red
	}
}

func main() {
	for l := Red; l <= Green; l++ {
		fmt.Println(l, int(l), "then", after(l))
	}
	fmt.Println("no name:", Light(7))
}
```

```output for=light
red 0 then green
amber 1 then red
green 2 then amber
no name: Light(7)
```

- Verdict: idiom. Go has no enum keyword. A named integer type and a `const` block with `iota` give the values 0, 1 and 2.
- A `String` method gives each value a name for printing.
- Any integer converts to `Light`, so `Light(7)` is a value with no name.
