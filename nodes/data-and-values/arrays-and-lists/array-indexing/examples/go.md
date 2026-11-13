```go run id=index-panic
package main

import "fmt"

func at(a []int, i int) (v int, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	return a[i], nil
}

func main() {
	a := []int{10, 20, 30, 40, 50}
	// Indices go from 0 to len(a)-1. Go has no negative indices.
	fmt.Println(a[0], a[len(a)-1], len(a))

	// An index out of range at run time panics. recover turns the panic into an error here.
	for _, i := range []int{4, 5, -1} {
		v, err := at(a, i)
		fmt.Println(i, v, err)
	}

	// A constant index that is out of range is a compile-time error, so the compiler
	// rejects a[-1] and a[5] on an array of length 5.
	arr := [5]int{1, 2, 3, 4, 5}
	i := 4
	fmt.Println(arr[i], len(arr))

	// A string index gives a byte, not a character.
	s := "héllo"
	fmt.Println(len(s), s[1], s[2], string([]rune(s)[1]))
}
```

```output
10 50 5
4 50 <nil>
5 0 runtime error: index out of range [5] with length 5
-1 0 runtime error: index out of range [-1]
5 5
6 195 169 é
```

- Verdict: native. Indices go from 0 to len(a)-1, and Go has no negative indices. An index out of range at run time panics with the index and the length, and recover turns the panic into an error. A constant index out of range is a compile-time error. A string index gives a byte, so "héllo" has 6 bytes.
