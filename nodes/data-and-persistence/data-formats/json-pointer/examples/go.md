```go run id=resolve
package main

import (
	"encoding/json"
	"fmt"
	"regexp"
	"strconv"
	"strings"
)

var index = regexp.MustCompile(`^(0|[1-9][0-9]*)$`)

// resolve returns the value that an RFC 6901 pointer identifies in doc.
func resolve(doc any, pointer string) (any, error) {
	if pointer == "" {
		return doc, nil
	}
	if !strings.HasPrefix(pointer, "/") {
		return nil, fmt.Errorf("bad pointer %q", pointer)
	}
	value := doc
	for _, raw := range strings.Split(pointer[1:], "/") {
		token := strings.ReplaceAll(strings.ReplaceAll(raw, "~1", "/"), "~0", "~")
		switch v := value.(type) {
		case map[string]any:
			next, ok := v[token]
			if !ok {
				return nil, fmt.Errorf("no member %q", token)
			}
			value = next
		case []any:
			if !index.MatchString(token) {
				return nil, fmt.Errorf("bad index %q", token)
			}
			i, err := strconv.Atoi(token)
			if err != nil || i >= len(v) {
				return nil, fmt.Errorf("no element %s", token)
			}
			value = v[i]
		default:
			return nil, fmt.Errorf("token %q applied to a %T", raw, value)
		}
	}
	return value, nil
}

func main() {
	var doc any
	json.Unmarshal([]byte(`{"servers": [{"host": "a.example", "port": 8080}], "m~n": true}`), &doc)
	for _, p := range []string{"/servers/0/host", "/servers/0/port", "/m~0n", "/servers/0/port/x", "/servers/-"} {
		v, err := resolve(doc, p)
		if err != nil {
			fmt.Println(p, "error:", err)
			continue
		}
		out, _ := json.Marshal(v)
		fmt.Println(p, string(out))
	}
}
```

```output for=resolve
/servers/0/host "a.example"
/servers/0/port 8080
/m~0n true
/servers/0/port/x error: token "x" applied to a float64
/servers/- error: bad index "-"
```

- Verdict: stdlib. `json.Unmarshal` into `any` gives `map[string]any`, `[]any` and scalar values, and a type switch walks them one token at a time. The package `encoding/json` has no function that resolves a JSON Pointer. A JSON number becomes a `float64`, so a token after the port number has no container to apply to. The token `-` names the place after the last element, so it is never a value.
