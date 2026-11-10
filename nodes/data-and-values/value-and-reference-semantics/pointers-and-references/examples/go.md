```go run id=change
package main

import "fmt"

func setCopy(n int) {
	n = 42
	fmt.Println("inside setCopy:", n)
}

func setPointer(p *int) {
	*p = 42
}

func main() {
	x := 1
	setCopy(x)
	fmt.Println("after setCopy:", x)
	setPointer(&x)
	fmt.Println("after setPointer:", x)

	p := &x
	q := &x
	fmt.Println("p == q:", p == q, "*p:", *p)

	array := [3]int{1, 2, 3}
	arrayCopy := array
	arrayCopy[0] = 100
	fmt.Println("array:", array)

	slice := []int{1, 2, 3}
	sliceAlias := slice
	sliceAlias[0] = 100
	fmt.Println("slice:", slice)
}
```

```output for=change
inside setCopy: 42
after setCopy: 1
after setPointer: 42
p == q: true *p: 42
array: [1 2 3]
slice: [100 2 3]
```

- Verdict: native. The type `*T` holds a pointer to a variable of type `T`. The operator `&` takes the address, and `*` reads or writes through the pointer.
- Every argument is passed by value. A function changes a variable of the caller only through a pointer.
- An array is a value, so the assignment copies all elements. A slice value refers to an underlying array, so the copy shares the elements.
- Go has no pointer arithmetic. `p == q` is true here because both pointers point to `x`.
