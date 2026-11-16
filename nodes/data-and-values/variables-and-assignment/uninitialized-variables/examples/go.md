```go run id=no-uninitialized
package main

import "fmt"

type Config struct {
	Name    string
	Retries int
}

func main() {
	var c Config
	fmt.Printf("%q %d\n", c.Name, c.Retries)

	var p *Config
	fmt.Println(p == nil)
	func() {
		defer func() {
			fmt.Println("recovered:", recover() != nil)
		}()
		fmt.Println(p.Name)
	}()
}
```

```output for=no-uninitialized
"" 0
true
recovered: true
```

- Verdict: absent. Go has no uninitialized variables: each variable starts at the zero value of its type. The risk moves to the nil pointer: `p` is nil, and the read of `p.Name` panics at run time. The deferred function recovers from the panic to show it.
