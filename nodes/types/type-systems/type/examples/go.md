```go run id=defined-types-and-type-switches
package main

import (
	"fmt"
	"reflect"
)

// Celsius is a new type with float64 as its underlying type.
type Celsius float64

// Temp is an alias: another name for the same type.
type Temp = float64

func (c Celsius) String() string { return fmt.Sprintf("%.1f°C", float64(c)) }

func main() {
	// Each type has a zero value.
	var i int
	var s string
	var p *int
	var m map[string]int
	fmt.Printf("%d %q %v %v\n", i, s, p, m == nil)

	// A defined type is different from its underlying type.
	var c Celsius = 21.5
	var f float64 = 1.5
	var t Temp = f // an alias needs no conversion
	c = c + Celsius(f) // a defined type needs one
	fmt.Println(c, t)
	fmt.Printf("%T %T %T\n", c, t, f)

	// An interface value holds a dynamic type and a value.
	var x any = c
	fmt.Println(reflect.TypeOf(x), reflect.TypeOf(x).Kind())
	for _, v := range []any{1, "two", 3.0, c, nil} {
		switch v := v.(type) {
		case int:
			fmt.Println("int", v+1)
		case string:
			fmt.Println("string", len(v))
		case fmt.Stringer:
			fmt.Println("Stringer", v.String())
		case nil:
			fmt.Println("nil")
		default:
			fmt.Printf("other %T\n", v)
		}
	}
}
```

```output for=defined-types-and-type-switches
0 "" <nil> true
23.0°C 1.5
main.Celsius float64 float64
main.Celsius float64
int 2
string 3
other float64
Stringer 23.0°C
nil
```

- Verdict: native. Each type has a zero value. `Celsius` is a defined type, so adding a `float64` needs the conversion `Celsius(f)`, and `Temp` is an alias, so it takes a `float64` with no conversion. `%T` prints `main.Celsius` for the defined type and `float64` for the alias. An interface value keeps its dynamic type, and a type switch selects a case by it, including the interface `fmt.Stringer`.
