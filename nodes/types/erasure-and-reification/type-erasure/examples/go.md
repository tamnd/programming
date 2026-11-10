```go run id=dict
package main

import (
	"fmt"
	"reflect"
)

type Celsius float64

func describe[T any](value T) string {
	switch any(value).(type) {
	case int:
		return "an int"
	case string:
		return "a string"
	}
	return "a " + reflect.TypeOf(value).String()
}

func name[T any]() string {
	return reflect.TypeFor[T]().String()
}

func main() {
	fmt.Println(describe(42))
	fmt.Println(describe("hi"))
	fmt.Println(describe(21.5))
	fmt.Println(describe(Celsius(21.5)))
	fmt.Println(name[[]Celsius](), name[error]())
}
```

```output for=dict
an int
a string
a float64
a main.Celsius
[]main.Celsius error
```

- Verdict: native. A generic function knows its type arguments at run time. The compiler passes them in a dictionary.
- `float64` and `Celsius` have the same underlying type, so they share one compiled copy of `describe`. The dictionary still tells the two types apart.
- `reflect.TypeFor[T]()` gives the type argument with no value at all, even for an interface type such as `error`.
