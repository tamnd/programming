```go run id=chunks-range-n-and-index-panic
package main

import "fmt"

func at(s []int, i int) (v int, err error) {
	defer func() {
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
		}
	}()
	return s[i], nil
}

func main() {
	data := []int{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}

	// Half-open slices: the high index of one chunk is the low of the next.
	for lo := 0; lo < len(data); lo += 4 {
		hi := min(lo+4, len(data))
		fmt.Println(lo, hi, data[lo:hi], "len", hi-lo)
	}

	// range over an integer gives 0 to n-1.
	var idx []int
	for i := range 3 {
		idx = append(idx, i)
	}
	fmt.Println(idx)

	// An index of len(s) is out of range, and Go panics.
	_, err := at(data, len(data))
	fmt.Println(err)
}
```

```output for=chunks-range-n-and-index-panic
0 4 [1 2 3 4] len 4
4 8 [5 6 7 8] len 4
8 10 [9 10] len 2
[0 1 2]
runtime error: index out of range [10] with length 10
```

- Verdict: native. The slice `data[lo:hi]` has the length `hi - lo`, and the chunks join with no gap because each `hi` is the next `lo`. The built-in `min` keeps the last `hi` at `len(data)`. `range 3` gives 0, 1 and 2. The index `len(data)` is one past the end, so Go panics, and the function turns the panic into an error with `recover`.
