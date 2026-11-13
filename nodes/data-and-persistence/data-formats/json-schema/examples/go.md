```go run id=enum-equality
package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"math/big"
)

// decode parses JSON and keeps each number as a json.Number, so no number loses precision.
func decode(text string) any {
	d := json.NewDecoder(bytes.NewReader([]byte(text)))
	d.UseNumber()
	var v any
	if err := d.Decode(&v); err != nil {
		panic(err)
	}
	return v
}

// equal follows the instance equality of JSON Schema: numbers by mathematical value,
// arrays item by item, and objects by their members in any order.
func equal(a, b any) bool {
	switch x := a.(type) {
	case json.Number:
		y, ok := b.(json.Number)
		if !ok {
			return false
		}
		rx, _ := new(big.Rat).SetString(string(x))
		ry, _ := new(big.Rat).SetString(string(y))
		return rx.Cmp(ry) == 0
	case []any:
		y, ok := b.([]any)
		if !ok || len(x) != len(y) {
			return false
		}
		for i := range x {
			if !equal(x[i], y[i]) {
				return false
			}
		}
		return true
	case map[string]any:
		y, ok := b.(map[string]any)
		if !ok || len(x) != len(y) {
			return false
		}
		for k, v := range x {
			w, found := y[k]
			if !found || !equal(v, w) {
				return false
			}
		}
		return true
	default: // nil, bool and string compare with ==.
		return a == b
	}
}

func main() {
	enum := decode(`[1, "a", null, {"x": [1, 2], "y": true}, 9007199254740993]`).([]any)
	for _, text := range []string{`1.0`, `10e-1`, `true`, `"A"`, `{"y": true, "x": [1.0, 2]}`, `{"x": [2, 1], "y": true}`, `9007199254740992`, `9007199254740993`} {
		v := decode(text)
		found := false
		for _, e := range enum {
			if equal(v, e) {
				found = true
				break
			}
		}
		fmt.Printf("%-28s %v\n", text, found)
	}
}
```

```output for=enum-equality
1.0                          true
10e-1                        true
true                         false
"A"                          false
{"y": true, "x": [1.0, 2]}   true
{"x": [2, 1], "y": true}     false
9007199254740992             false
9007199254740993             true
```

- Verdict: library. The Go standard library has no JSON Schema validator. The example implements the instance equality of the core specification, which the keywords `enum` and `const` use. Two numbers are equal when they have the same mathematical value, so `1.0` and `10e-1` match `1`. A boolean is never equal to a number. Two objects are equal when they have the same members in any order, and two arrays are equal item by item in order. The decoder uses `UseNumber`, and `math/big` compares the numbers exactly. With `float64`, the numbers 9007199254740992 and 9007199254740993 become the same value, and the wrong number matches.
