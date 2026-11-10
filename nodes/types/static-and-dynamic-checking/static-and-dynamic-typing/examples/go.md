```go run id=types
package main

import "fmt"

func describe(value any) string {
	switch v := value.(type) {
	case int:
		return fmt.Sprintf("int %d", v+1)
	case string:
		return fmt.Sprintf("string of length %d", len(v))
	default:
		return fmt.Sprintf("other %T", v)
	}
}

func main() {
	var value any = 42
	fmt.Printf("%T\n", value)
	for _, v := range []any{41, "abc", 2.5} {
		fmt.Println(describe(v))
	}
	s, ok := value.(string)
	fmt.Printf("%q %v\n", s, ok)
}
```

```output
int
int 42
string of length 3
other float64
"" false
```

- Verdict: native. Go is statically typed. `:=` infers the type of a variable at compile time.
- A variable of interface type, such as `any`, has a static type and a dynamic type. A type switch or a type assertion reads the dynamic type.
- The form `s, ok := value.(string)` gives `ok == false` on a wrong type. The single-value form panics.

```go fails expect=panic id=assert
package main

import "fmt"

func main() {
	var value any = 42
	s := value.(string)
	fmt.Println(s)
}
```
