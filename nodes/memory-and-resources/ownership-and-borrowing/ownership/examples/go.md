```go run id=clone
package main

import (
	"fmt"
	"slices"
)

func main() {
	all := []int{1, 2, 3, 4}
	head := slices.Clone(all[:2])
	head = append(head, 99)
	fmt.Println(all, head)

	limited := all[:2:2]
	limited = append(limited, 7)
	fmt.Println(all, limited)
}
```

```output for=clone
[1 2 3 4] [1 2 99]
[1 2 3 4] [1 2 7]
```

- Verdict: absent. The garbage collector frees memory, and a slice is a view of a shared array. A slice does not show if it owns its array.
- `slices.Clone` gives the new slice its own array. A full slice expression, `s[low:high:max]`, limits the capacity, so that `append` must copy.

```go wrong id=shared-append
package main

import "fmt"

func main() {
	all := []int{1, 2, 3, 4}
	head := all[:2]
	head = append(head, 99)
	fmt.Println(all)
}
```

```output for=shared-append
[1 2 99 4]
```

`head` shares the array of `all` and has spare capacity, so `append` writes over the third element of `all`.
