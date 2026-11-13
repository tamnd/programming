```go run id=config-reader
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

// Shallow: each step is its own function; the caller must know them all.
func splitLines(text string) []string { return strings.Split(text, "\n") }

func dropComments(lines []string) []string {
	var out []string
	for _, l := range lines {
		if !strings.HasPrefix(l, "#") {
			out = append(out, l)
		}
	}
	return out
}

func dropBlank(lines []string) []string {
	var out []string
	for _, l := range lines {
		if strings.TrimSpace(l) != "" {
			out = append(out, l)
		}
	}
	return out
}

func toPair(line string) (string, string) {
	k, v, _ := strings.Cut(line, "=")
	return strings.TrimSpace(k), strings.TrimSpace(v)
}

// parseConfig is deep: one simple interface hides the same steps.
func parseConfig(text string) map[string]string {
	result := map[string]string{}
	for _, line := range strings.Split(text, "\n") {
		s := strings.TrimSpace(line)
		if s == "" || strings.HasPrefix(s, "#") {
			continue
		}
		k, v, _ := strings.Cut(s, "=")
		result[strings.TrimSpace(k)] = strings.TrimSpace(v)
	}
	return result
}

func main() {
	text := "# server\nhost = example.org\n\nport = 8080\n"
	shallow := map[string]string{}
	for _, l := range dropBlank(dropComments(splitLines(text))) {
		k, v := toPair(l)
		shallow[k] = v
	}
	s, _ := json.Marshal(shallow) // map keys are sorted
	d, _ := json.Marshal(parseConfig(text))
	fmt.Printf("shallow: %s\n", s)
	fmt.Printf("deep: %s\n", d)
}
```

```output
shallow: {"host":"example.org","port":"8080"}
deep: {"host":"example.org","port":"8080"}
```
