```go run id=sprintf
package main

import (
	"fmt"
	"strconv"
)

func main() {
	name := "Ada"
	price := 4.5
	items := []string{"tea", "milk"}

	s := fmt.Sprintf("%s pays %.2f for %d items.", name, price*2, len(items))
	fmt.Println(s)
	fmt.Println(name + " has " + strconv.Itoa(len(items)) + " items.")
	fmt.Printf("%[1]s and %[1]q\n", name)
	fmt.Println("{name} and ${name} are text.")
}
```

```output for=sprintf
Ada pays 9.00 for 2 items.
Ada has 2 items.
Ada and "Ada"
{name} and ${name} are text.
```

- Verdict: idiom. Go has no string interpolation. `fmt.Sprintf` takes a format string with verbs such as `%s`, `%d` and `%.2f`, and the values after it. The operator `+` joins strings only, so a number goes through `strconv` first. `%[1]s` takes the first value again.
