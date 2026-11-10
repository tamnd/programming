```go run id=request
package main

import (
	"errors"
	"fmt"
	"strings"
)

type Request struct {
	Method  string
	URL     string
	Headers []string
	Timeout int
	Retries int
}

// An Option sets one part of a Request.
type Option func(*Request)

func WithMethod(method string) Option { return func(r *Request) { r.Method = method } }
func WithHeader(header string) Option { return func(r *Request) { r.Headers = append(r.Headers, header) } }
func WithTimeout(seconds int) Option  { return func(r *Request) { r.Timeout = seconds } }
func WithRetries(retries int) Option  { return func(r *Request) { r.Retries = retries } }

func NewRequest(url string, options ...Option) (Request, error) {
	request := Request{Method: "GET", URL: url, Timeout: 30}
	for _, option := range options {
		option(&request)
	}
	if request.Timeout <= 0 {
		return Request{}, errors.New("timeout must be positive")
	}
	return request, nil
}

func show(r Request) {
	fmt.Printf("%s %s timeout=%d retries=%d headers=[%s]\n", r.Method, r.URL, r.Timeout, r.Retries, strings.Join(r.Headers, ", "))
}

func main() {
	orders, _ := NewRequest("https://example.com/orders")
	show(orders)
	post, _ := NewRequest("https://example.com/orders",
		WithMethod("POST"), WithHeader("Accept: application/json"), WithTimeout(5), WithRetries(2))
	show(post)
	if _, err := NewRequest("https://example.com/orders", WithTimeout(0)); err != nil {
		fmt.Println("error:", err)
	}
}
```

```output for=request
GET https://example.com/orders timeout=30 retries=0 headers=[]
POST https://example.com/orders timeout=5 retries=2 headers=[Accept: application/json]
error: timeout must be positive
```

- Verdict: idiom. Go has no named, default or optional arguments. Functional options are a common form: each option is a function that sets one part, and the constructor takes any number of them.
- The constructor sets the defaults first and then applies the options in order. A later option overrides an earlier one.
- The constructor returns an error in place of an invalid request, as a `build` method with a check does.
- A second common form is a struct of options passed by value, such as `Options{Timeout: 5}`. Fields that the caller leaves out get their zero value, so a default other than zero needs a rule such as "zero means 30".
