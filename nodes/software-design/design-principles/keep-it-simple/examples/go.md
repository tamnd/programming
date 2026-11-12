```go run id=simple-config
package main

import (
	"fmt"
	"strings"
)

func parseConfig(src string) map[string]string {
	out := map[string]string{}
	for _, line := range strings.Split(src, "\n") {
		if line == "" || strings.HasPrefix(line, "#") {
			continue
		}
		key, value, _ := strings.Cut(line, "=")
		out[key] = value
	}
	return out
}

func main() {
	fmt.Println(parseConfig("host=example.org\nport=8080\n# a comment\n"))
}
```

```output
map[host:example.org port:8080]
```
