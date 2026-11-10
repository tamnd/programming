```go run id=pipeline
package main

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"strings"
)

// Middleware is the usual Go form of a link: it takes the rest of the chain
// and returns a handler that does its own work and then may call next.
type Middleware func(next http.Handler) http.Handler

func logRequests(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		user := r.Header.Get("X-User")
		if user == "" {
			user = "nobody"
		}
		fmt.Printf("log: %s from %s\n", r.URL.Path, user)
		next.ServeHTTP(w, r)
	})
}

func requireUser(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if r.Header.Get("X-User") == "" {
			http.Error(w, "sign in first", http.StatusUnauthorized)
			return
		}
		next.ServeHTTP(w, r)
	})
}

func cache(next http.Handler) http.Handler {
	saved := map[string]string{}
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		key := r.Header.Get("X-User") + " " + r.URL.Path
		if body, ok := saved[key]; ok {
			fmt.Fprint(w, body+" (cached)")
			return
		}
		record := httptest.NewRecorder()
		next.ServeHTTP(record, r)
		saved[key] = record.Body.String()
		w.WriteHeader(record.Code)
		fmt.Fprint(w, record.Body.String())
	})
}

func app(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "report for %s", r.Header.Get("X-User"))
}

// chain wraps from the last middleware to the first, so the first one runs first.
func chain(last http.Handler, middleware ...Middleware) http.Handler {
	for i := len(middleware) - 1; i >= 0; i-- {
		last = middleware[i](last)
	}
	return last
}

func main() {
	serve := chain(http.HandlerFunc(app), logRequests, requireUser, cache)
	for _, user := range []string{"ada", "", "ada"} {
		r := httptest.NewRequest(http.MethodGet, "/report", nil)
		if user != "" {
			r.Header.Set("X-User", user)
		}
		w := httptest.NewRecorder()
		serve.ServeHTTP(w, r)
		fmt.Println(w.Code, strings.TrimSpace(w.Body.String()))
	}
}
```

```output
log: /report from ada
200 report for ada
log: /report from nobody
401 sign in first
log: /report from ada
200 report for ada (cached)
```

- Verdict: stdlib. `net/http` has no chain type, but `http.Handler` and `http.HandlerFunc` make a link a function of type `func(http.Handler) http.Handler`. Go web code calls this middleware.
- `httptest.NewRequest` and `httptest.NewRecorder` drive the chain without a network. The cache also uses a recorder to capture the answer of the rest of the chain.
- `http.Error` ends its body with a newline, so `main` trims the body before it prints it.
- The cache map has no lock. A real server calls handlers from many goroutines, so it needs a `sync.Mutex` around the map.
