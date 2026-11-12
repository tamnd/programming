```go run id=language-features
package main

import (
	"bytes"
	"fmt"
	"sync"
)

type Request struct {
	URL     string
	Method  string
	Retries int
}

// NewRequest is a constructor function: it fills in the defaults.
func NewRequest(url string) *Request {
	return &Request{URL: url, Method: "GET"}
}

// config is made once, on the first call.
var config = sync.OnceValue(func() string {
	fmt.Println("loading config")
	return "prod"
})

func main() {
	// The zero value is ready to use, so no constructor is needed.
	var buf bytes.Buffer
	buf.WriteString("ready to use")
	fmt.Println("zero value:", buf.String())

	// A composite literal names the fields.
	r := Request{URL: "https://example.com/", Retries: 2}
	fmt.Printf("literal: %+v\n", r)
	fmt.Printf("constructor: %+v\n", *NewRequest("https://example.com/"))

	// Assignment copies a struct, so a value is its own prototype.
	c := r
	c.Method = "POST"
	fmt.Printf("copy: %q %q\n", r.Method, c.Method)

	fmt.Println("singleton:", config(), config())
}
```

```output
zero value: ready to use
literal: {URL:https://example.com/ Method: Retries:2}
constructor: {URL:https://example.com/ Method:GET Retries:0}
copy: "" "POST"
loading config
singleton: prod prod
```

- Verdict: idiom. Go has no constructors. A type whose zero value is ready to use needs no creation code, a composite literal names the fields, and a function named `NewT` is the constructor when the defaults matter. Assignment copies a struct value. `sync.OnceValue` makes a value once.
