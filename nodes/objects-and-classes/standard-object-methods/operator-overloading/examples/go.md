```go run id=methods-and-struct-equality
package main

import "fmt"

type Money struct {
	Cents int64
}

func (m Money) Add(other Money) Money {
	return Money{m.Cents + other.Cents}
}

func (m Money) Less(other Money) bool {
	return m.Cents < other.Cents
}

func main() {
	a, b := Money{150}, Money{250}
	fmt.Println(a.Add(b))
	fmt.Println(a.Add(b) == Money{400}, a.Less(b))

	seen := map[Money]bool{a: true}
	fmt.Println(seen[Money{150}])
}
```

```output for=methods-and-struct-equality
{400}
true true
true
```

- Verdict: absent. Go has no user operators, so `Money` has the methods `Add` and `Less`.
- `==` is built in for a struct whose fields are comparable: it compares the fields. Such a struct can also be a map key.
- `<` does not work on structs; a sort takes a function such as `func(x, y Money) int`.
