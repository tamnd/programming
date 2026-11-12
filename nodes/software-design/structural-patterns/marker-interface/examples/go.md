```go run id=redacting-logger
package main

import "fmt"

type Value interface {
	Text() string
}

// Sensitive has one unexported method, so only types in this package
// can be marked. An interface with no methods would match every type.
type Sensitive interface {
	sensitive()
}

type Username struct{ value string }
type Password struct{ value string }
type ApiKey struct{ value string }

func (u Username) Text() string { return u.value }
func (p Password) Text() string { return p.value }
func (k ApiKey) Text() string   { return k.value }

func (Password) sensitive() {}
func (ApiKey) sensitive()   {}

func log(label string, item Value) {
	shown := item.Text()
	if _, ok := item.(Sensitive); ok {
		shown = "[redacted]"
	}
	fmt.Printf("%s: %s\n", label, shown)
}

func main() {
	log("user", Username{"ann"})
	log("password", Password{"hunter2"})
	log("api key", ApiKey{"k-123"})
}
```

```output
user: ann
password: [redacted]
api key: [redacted]
```

- Verdict: idiom. The marker is an interface with one empty, unexported method, as `go/ast` does with `exprNode()`. A type assertion tests it at run time.
