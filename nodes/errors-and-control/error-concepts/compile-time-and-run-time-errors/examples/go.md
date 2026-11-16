```go run id=when-go
package main

import "fmt"

func get(items []int, i int) (v int, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	return items[i], nil
}

func main() {
	items := []int{10, 20, 30}
	for _, i := range []int{1, 5} {
		v, err := get(items, i)
		if err != nil {
			fmt.Printf("items[%d]: %v\n", i, err)
			continue
		}
		fmt.Printf("items[%d] = %d\n", i, v)
	}
}
```

```output for=when-go
items[1] = 20
items[5]: runtime error: index out of range [5] with length 3
```

- Verdict: native. The Go compiler finds syntax and type errors, and the spec makes an import that the package does not use illegal. The compiler rejects a constant index that is out of range for an array. A variable index is tested at run time, and a bad one causes a run-time panic, which `recover` turns into an error here.
