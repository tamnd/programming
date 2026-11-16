```go run id=comparable-struct-and-stringer
package main

import "fmt"

type Money struct {
	Cents    int64
	Currency string
}

// String makes Money a fmt.Stringer.
func (m Money) String() string {
	return fmt.Sprintf("%d.%02d %s", m.Cents/100, m.Cents%100, m.Currency)
}

type Plain struct{ X, Y int }

func main() {
	a, b := Money{1250, "EUR"}, Money{1250, "EUR"}
	fmt.Println(a == b)
	names := map[Money]string{a: "lunch"}
	fmt.Println(names[b], a)
	fmt.Printf("%v %+v\n", Plain{1, 2}, Plain{1, 2})
}
```

```output for=comparable-struct-and-stringer
true
lunch 12.50 EUR
{1 2} {X:1 Y:2}
```

- Verdict: idiom. Go has no root type. `==` works on any struct whose fields are all comparable, and it compares the fields, so a struct like `Money` can be a map key with no code.
- The `fmt` package calls the `String` method of a value that implements `fmt.Stringer`. Without one, `%v` prints the fields in braces and `%+v` adds their names.
- Code cannot change `==` or the hash of a map key. A type with a slice or map field is not comparable; give it an `Equal` method, and use a comparable key in maps.
