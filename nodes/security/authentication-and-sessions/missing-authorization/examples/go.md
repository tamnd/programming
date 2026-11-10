```go run since=1.22 id=middleware
package main

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	"strconv"
	"strings"
)

type invoice struct {
	Owner string
	Total int
}

var (
	sessions = map[string]string{"session-alice": "alice", "session-carol": "carol"}
	roles    = map[string]string{"alice": "customer", "bob": "customer", "carol": "admin"}
	// policy lists what each role may do. Anything not listed is denied.
	policy = map[string]map[string]bool{
		"customer": {"invoice:read": true},
		"admin":    {"invoice:read": true, "user:delete": true},
	}
	invoices = map[int]invoice{1: {"alice", 120}, 2: {"bob", 75}}
)

type userHandler func(w http.ResponseWriter, r *http.Request, user string)

// handle registers a route only together with the permission that it needs.
func handle(mux *http.ServeMux, pattern, permission string, h userHandler) {
	mux.HandleFunc(pattern, func(w http.ResponseWriter, r *http.Request) {
		user, ok := "", false
		if c, err := r.Cookie("session"); err == nil {
			user, ok = sessions[c.Value]
		}
		if !ok {
			http.Error(w, "log in first", http.StatusUnauthorized)
			return
		}
		if !policy[roles[user]][permission] {
			http.Error(w, "forbidden", http.StatusForbidden)
			return
		}
		h(w, r, user)
	})
}

func getInvoice(w http.ResponseWriter, r *http.Request, user string) {
	id, err := strconv.Atoi(r.PathValue("id"))
	inv, ok := invoices[id]
	// Another user's invoice gets the same answer as a missing one.
	if err != nil || !ok || inv.Owner != user {
		http.NotFound(w, r)
		return
	}
	fmt.Fprintf(w, "invoice %d total %d", id, inv.Total)
}

func deleteUser(w http.ResponseWriter, r *http.Request, user string) {
	fmt.Fprintf(w, "deleted %s", r.PathValue("name"))
}

func main() {
	mux := http.NewServeMux()
	handle(mux, "GET /invoices/{id}", "invoice:read", getInvoice)
	handle(mux, "DELETE /users/{name}", "user:delete", deleteUser)

	// httptest calls the handler in memory. Nothing goes over the network.
	send := func(method, path, session string) {
		req := httptest.NewRequest(method, path, nil)
		req.AddCookie(&http.Cookie{Name: "session", Value: session})
		rec := httptest.NewRecorder()
		mux.ServeHTTP(rec, req)
		fmt.Printf("%-6s %-15s %-14s %d %s\n", method, path, session, rec.Code, strings.TrimSpace(rec.Body.String()))
	}
	send("GET", "/invoices/1", "session-alice")
	send("GET", "/invoices/2", "session-alice")
	send("DELETE", "/users/bob", "session-alice")
	send("DELETE", "/users/bob", "session-carol")
	send("GET", "/invoices/1", "none")
}
```

```output for=middleware
GET    /invoices/1     session-alice  200 invoice 1 total 120
GET    /invoices/2     session-alice  404 404 page not found
DELETE /users/bob      session-alice  403 forbidden
DELETE /users/bob      session-carol  200 deleted bob
GET    /invoices/1     none           401 log in first
```

`handle` is the only way to add a route, and it takes the permission as an argument. The handler runs only after the check passes. `getInvoice` compares the owner with the caller, and answers 404 for another user's invoice.

- Verdict: stdlib. `net/http` has no authorization, but a function that wraps a handler is the usual place for the check. `httptest` runs the handlers in memory, with no network.
- Method and wildcard patterns such as `GET /invoices/{id}` and `r.PathValue` need Go 1.22 or later.
- A route that someone adds with `mux.HandleFunc` directly skips the check. Review for such calls, or keep the `ServeMux` unexported behind `handle`.
