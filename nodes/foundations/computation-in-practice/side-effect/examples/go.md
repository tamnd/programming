```go run id=what-a-call-can-change
package main

import "fmt"

type Account struct {
	Owner   string
	Balance int
}

func depositValue(a Account, n int) { a.Balance += n } // changes a copy

func depositPointer(a *Account, n int) { a.Balance += n } // changes the caller's struct

func resetFirst(xs []int) { xs[0] = 0 } // a slice shares its array with the caller

func count(words []string, seen map[string]int) {
	for _, w := range words {
		seen[w]++ // a map is a reference to shared data
	}
}

func main() {
	acc := Account{"Ada", 100}
	depositValue(acc, 50)
	fmt.Println("after depositValue:", acc.Balance)
	depositPointer(&acc, 50)
	fmt.Println("after depositPointer:", acc.Balance)

	xs := []int{7, 8, 9}
	resetFirst(xs)
	fmt.Println("after resetFirst:", xs)

	seen := map[string]int{}
	count([]string{"a", "b", "a"}, seen)
	fmt.Println("seen:", seen)
}
```

```output for=what-a-call-can-change
after depositValue: 100
after depositPointer: 150
after resetFirst: [0 8 9]
seen: map[a:2 b:1]
```

- Verdict: native. Go passes every argument by value, so a struct parameter is a copy, and `depositValue` changes only its copy. A pointer parameter shows that the function can change the caller's value. Slices and maps are small values that point to shared data, so a function that receives them can change the caller's elements with no pointer in the signature. `append` can also reuse the caller's array when it has spare capacity.
