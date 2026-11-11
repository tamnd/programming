```go run id=middleware
package main

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"strings"
)

type middleware func(http.Handler) http.Handler

func logging(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("log:", r.Method, r.URL.Path)
		next.ServeHTTP(w, r)
	})
}

func requireKey(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if r.Header.Get("X-Key") != "secret" {
			http.Error(w, "forbidden", http.StatusForbidden)
			return
		}
		next.ServeHTTP(w, r)
	})
}

func chain(h http.Handler, ms ...middleware) http.Handler {
	for i := len(ms) - 1; i >= 0; i-- {
		h = ms[i](h)
	}
	return h
}

func main() {
	hello := http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprint(w, "hello")
	})
	app := chain(hello, logging, requireKey)

	for _, key := range []string{"", "secret"} {
		req := httptest.NewRequest("GET", "/hello", nil)
		req.Header.Set("X-Key", key)
		rec := httptest.NewRecorder()
		app.ServeHTTP(rec, req)
		fmt.Println(rec.Code, strings.TrimSpace(rec.Body.String()))
	}
}
```

```output for=middleware
log: GET /hello
403 forbidden
log: GET /hello
200 hello
```

- Verdict: idiom. Go has no aspects, annotations or proxies. Middleware is the Go form of around advice: a function that takes a handler and returns a handler that does work before and after it, or does not call it at all. The concern lives in one place, and `chain` applies it to any handler. The selection of join points is explicit: only the handlers passed through `chain` get the advice.
