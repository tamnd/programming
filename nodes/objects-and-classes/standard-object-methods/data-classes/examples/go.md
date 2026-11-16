```go run id=plain-struct
package main

import "fmt"

type Version struct {
	Major int
	Minor int
}

func main() {
	a := Version{Major: 1, Minor: 4}
	fmt.Printf("%v %+v\n", a, a)
	fmt.Println(a == Version{1, 4})

	b := a
	b.Minor = 5
	fmt.Println(a.Minor, b.Minor)

	seen := map[Version]bool{a: true}
	fmt.Println(seen[Version{1, 4}], len(seen))
}
```

```output for=plain-struct
{1 4} {Major:1 Minor:4}
true
4 5
true 1
```

- Verdict: idiom. Go has no data classes, but a plain struct already has most of their behavior: `==` compares the fields, and `fmt` prints them with `%v` and `%+v`.
- Assignment copies a struct, so `b := a` followed by a change is a copy with changes.
- A struct with comparable fields can be a map key. A struct with a slice or a map field is not comparable, and `==` does not compile for it.
