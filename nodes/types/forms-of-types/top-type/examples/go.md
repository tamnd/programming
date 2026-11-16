```go run id=any-interface
package main

import "fmt"

func describe(v any) string {
	switch x := v.(type) {
	case int:
		return fmt.Sprint("int ", x+1)
	case string:
		return "string " + x
	case []int:
		return fmt.Sprint("[]int ", len(x))
	case nil:
		return "nil"
	default:
		return fmt.Sprintf("%T", x)
	}
}

func main() {
	values := []any{42, "hi", []int{1, 2}, nil, 3.5}
	for _, v := range values {
		fmt.Println(describe(v))
	}

	// The form with two results reports a failed assertion.
	var v any = 42
	s, ok := v.(string)
	fmt.Printf("%q %v\n", s, ok)

	// The form with one result panics.
	defer func() {
		fmt.Println("recovered:", recover())
	}()
	_ = v.(string)
}
```

```output for=any-interface
int 43
string hi
[]int 2
nil
float64
"" false
recovered: interface conversion: interface {} is int, not string
```

- Verdict: native. `any` is the empty interface, and every type implements it, so a `[]any` can hold an integer, a string, a slice and `nil`. A type switch picks the case for the dynamic type. A type assertion with two results returns `false` when the type is wrong, and the form with one result panics.
