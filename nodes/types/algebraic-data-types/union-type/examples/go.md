```go run id=either
package main

import (
	"fmt"
	"strings"
)

func describe(value any) string {
	switch v := value.(type) {
	case int:
		return fmt.Sprintf("number %d, doubled %d", v, v*2)
	case string:
		return fmt.Sprintf("text %s, upper %s", v, strings.ToUpper(v))
	default:
		return fmt.Sprintf("not a number or a string: %T", v)
	}
}

type intOrString interface {
	int | string
}

func double[T intOrString](x T) T {
	return x + x
}

func main() {
	for _, v := range []any{21, "ok"} {
		fmt.Println(describe(v))
	}
	fmt.Println("constraint:", double(21), double("ok"))
}
```

```output for=either
number 21, doubled 42
text ok, upper OK
constraint: 42 okok
```

- Verdict: idiom. A variable of type `any` holds a value of any type, and a type switch finds the actual type.
- The interface `intOrString` lists the types `int | string`, but it can only be a type constraint, as in `double`. A variable cannot have that type.
- `+` works in `double` because both `int` and `string` support it.
