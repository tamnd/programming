```go run id=replaceable-function-variable
package main

import (
	"fmt"
	"time"
)

// now is a variable, not a method, so other code can replace it.
var now = time.Now

func greeting() string {
	if now().Hour() < 12 {
		return "Good morning"
	}
	return "Good afternoon"
}

func main() {
	saved := now
	defer func() { now = saved }()

	now = func() time.Time { return time.Date(2026, 1, 2, 9, 0, 0, 0, time.UTC) }
	fmt.Println(greeting())
	now = func() time.Time { return time.Date(2026, 1, 2, 15, 0, 0, 0, time.UTC) }
	fmt.Println(greeting())
}
```

```output for=replaceable-function-variable
Good morning
Good afternoon
```

Go cannot add a method to a type of another package or replace a method at run time. The spec says that "The method set of a type determines the methods that can be called on an operand of that type", and the method set is part of the compiled program. Code that must be replaceable keeps the behaviour in a variable of a function type, as `now` is here, or takes an interface as a parameter. A test can then assign a fixed clock to `now` and restore the old value with `defer`.
