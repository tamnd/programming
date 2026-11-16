```go run id=exhaustive-matching-go
package main

import "fmt"

type Light int

const (
	Red Light = iota
	Yellow
	Green
)

func action(l Light) string {
	switch l {
	case Red:
		return "stop"
	case Yellow:
		return "slow down"
	case Green:
		return "go"
	default:
		panic(fmt.Sprintf("unknown light %d", l))
	}
}

func main() {
	names := []string{"red", "yellow", "green"}
	for l := Red; l <= Green; l++ {
		fmt.Printf("%s: %s\n", names[l], action(l))
	}
}
```

```output for=exhaustive-matching-go
red: stop
yellow: slow down
green: go
```

- Verdict: absent. Go has no enum types and no exhaustiveness check. If no case matches and there is no `default`, the switch does nothing. A `default` that panics finds a missing case at run time.
