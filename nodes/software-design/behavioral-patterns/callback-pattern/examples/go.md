```go run id=callbacks
package main

import (
	"fmt"
	"strings"
)

func forEach(items []string, callback func(string)) {
	for _, item := range items {
		callback(item) // a blocking callback
	}
}

func main() {
	forEach([]string{"hello", "world"}, func(w string) { fmt.Println(strings.ToUpper(w)) })

	var pending []func()
	pending = append(pending, func() { fmt.Println("saved") }) // a deferred callback
	fmt.Println("request sent")
	for _, f := range pending {
		f()
	}
}
```

```output for=callbacks
HELLO
WORLD
request sent
saved
```

- Verdict: native. Functions are values, and `sort.Slice` takes a less function in the same way.
