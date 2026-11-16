```go run id=reflect-and-init-registry
package main

import (
	"fmt"
	"reflect"
	"sort"
	"strings"
)

type Plugin interface{ Run(text string) string }

type Upper struct{}

func (Upper) Run(text string) string { return strings.ToUpper(text) }

type Reverse struct{}

func (Reverse) Run(text string) string {
	r := []rune(text)
	for i, j := 0, len(r)-1; i < j; i, j = i+1, j-1 {
		r[i], r[j] = r[j], r[i]
	}
	return string(r)
}

var registry = map[string]Plugin{}

// register plays the part of a metaclass that registers each class.
func register(p Plugin) {
	registry[strings.ToLower(reflect.TypeOf(p).Name())] = p
}

func init() {
	register(Upper{})
	register(Reverse{})
}

func main() {
	t := reflect.TypeOf(Upper{}) // the value that describes the type at run time
	fmt.Println(t.Name(), t.Kind(), t.NumMethod())
	names := make([]string, 0, len(registry))
	for name := range registry {
		names = append(names, name)
	}
	sort.Strings(names)
	fmt.Println(names, registry["reverse"].Run("abc"))
}
```

```output for=reflect-and-init-registry
Upper struct 1
[reverse upper] cba
```

- Verdict: absent. Go has no classes, and `reflect.TypeOf` gives a value that describes a type, with its name, its kind and its methods.
- An `init` function runs before `main`, so each plugin can register itself there. The registration is a call in the code, not a rule that the language applies to each type.
