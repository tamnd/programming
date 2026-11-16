```go run id=function-or-defined-type
package main

import (
	"fmt"
	"strings"
)

// func (s string) WordCount() int { ... } does not compile:
// a method needs a receiver type that is declared in this package.

// WordCount is a plain function: the usual answer in Go.
func WordCount(s string) int { return len(strings.Fields(s)) }

// Text is a new type with string as its underlying type, so it can have methods.
type Text string

func (t Text) WordCount() int { return WordCount(string(t)) }

func main() {
	s := "Hello defined types"
	fmt.Println(WordCount(s), Text(s).WordCount(), strings.ToUpper(string(Text("ok"))))
}
```

```output for=function-or-defined-type
3 3 OK
```

- Verdict: absent. A method must be declared in the package of its receiver type, so code cannot add a method to `string` or to a type from another package.
- A plain function is the usual answer. A new type such as `Text` can have methods, but a `Text` is not a `string`: the code must convert values from one type to the other.
