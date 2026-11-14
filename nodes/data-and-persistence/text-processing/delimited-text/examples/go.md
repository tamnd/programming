```go run id=split-fields
package main

import (
	"fmt"
	"strings"
)

func main() {
	// Split keeps every empty field, also the last one.
	fmt.Printf("%q\n", strings.Split("a::b:", ":"))
	fmt.Printf("%q\n", strings.Split("", ":"))

	// Fields splits at runs of white space and drops the ends.
	fmt.Printf("%q\n", strings.Fields("  a  b "))

	// SplitN with n=2 keeps the rest of the line in the last field.
	fmt.Printf("%q\n", strings.SplitN("k=v=w", "=", 2))
}
```

```output for=split-fields
["a" "" "b" ""]
[""]
["a" "b"]
["k" "v=w"]
```

- Verdict: stdlib. `strings.Split` keeps each empty field. `strings.Fields` splits at runs of white space and returns only fields that are not empty. `strings.SplitN` with 2 keeps the rest of the line, with its separators, in the last field. The `encoding/csv` package reads fields with quotes.
