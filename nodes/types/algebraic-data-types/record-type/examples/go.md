```go run id=record
package main

import "fmt"

type Person struct {
	Name string
	Age  int
}

func yn(b bool) string {
	if b {
		return "yes"
	}
	return "no"
}

func main() {
	ada := Person{Name: "Ada", Age: 36}
	fmt.Println("name:", ada.Name, "age:", ada.Age)

	older := ada // assignment copies every field
	older.Age++
	fmt.Printf("copy: %+v\n", older)
	fmt.Printf("original: %+v\n", ada)
	fmt.Println("same fields equal:", yn(ada == Person{"Ada", 36}))
}
```

```output for=record
name: Ada age: 36
copy: {Name:Ada Age:37}
original: {Name:Ada Age:36}
same fields equal: yes
```

- Verdict: native. A `struct` is a record. A struct value is copied on assignment and when it is passed to a function.
- `==` compares the fields when all field types are comparable.
- Go has no read-only fields. To keep a record unchanged, pass it by value or keep its fields unexported.
