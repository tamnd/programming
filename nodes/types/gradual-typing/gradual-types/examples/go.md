```go run id=any-and-type-assertions
package main

import (
	"encoding/json"
	"fmt"
)

func main() {
	var doc any // the empty interface: a value of any type
	if err := json.Unmarshal([]byte(`{"name": "ada", "year": 1815, "tags": ["math"]}`), &doc); err != nil {
		panic(err)
	}
	m := doc.(map[string]any) // a type assertion, checked when it runs
	for _, key := range []string{"name", "year", "tags"} {
		switch v := m[key].(type) {
		case string:
			fmt.Println(key, "is a string:", v)
		case float64: // JSON numbers decode as float64
			fmt.Println(key, "is a float64:", v)
		case []any:
			fmt.Println(key, "is a list of", len(v))
		}
	}
	if _, ok := m["year"].(int); !ok {
		fmt.Println("year is not an int")
	}
}
```

```output for=any-and-type-assertions
name is a string: ada
year is a float64: 1815
tags is a list of 1
year is not an int
```

- Verdict: idiom. Go is statically typed, with `any`, the empty interface, as the type that holds a value of any type. A type assertion or a type switch gets the concrete type back and is checked when it runs: the form `v, ok := x.(T)` reports failure, and the form without `ok` panics. JSON decoded into `any` shows the cost of losing the static type: numbers come back as `float64`, so the assertion to `int` fails.
