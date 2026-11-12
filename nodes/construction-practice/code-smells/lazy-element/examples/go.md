```go run id=inline-the-layers
package main

import "fmt"

type person struct{ first, last string }

// Before: an interface with one implementation, and one method that does little.
type nameFormatter interface {
	Format(p person) string
}

type spaceFormatter struct{}

func (spaceFormatter) Format(p person) string { return p.first + " " + p.last }

func greetingBefore(f nameFormatter, p person) string {
	return "Hello, " + f.Format(p)
}

// After: one function, and no interface to pass around.
func fullName(p person) string { return p.first + " " + p.last }

func greeting(p person) string { return "Hello, " + fullName(p) }

func main() {
	ada := person{"Ada", "Lovelace"}
	if greetingBefore(spaceFormatter{}, ada) != "Hello, Ada Lovelace" {
		panic("before")
	}
	if greeting(ada) != greetingBefore(spaceFormatter{}, ada) {
		panic("after")
	}
	fmt.Println("greeting: tests pass")
}
```

```output
greeting: tests pass
```

- In Go, an interface with one implementation is a common form of this smell. Each caller must pass the implementation, and the reader must find it to learn what `Format` does.
- If a test needs a fake formatter, the interface has a second user, and it is not lazy. Keep it then.
