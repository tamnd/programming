```go wrong id=pprof
package main

import (
	"fmt"
	"net/http"
	"net/http/httptest"
	_ "net/http/pprof" // registers /debug/pprof/ on http.DefaultServeMux
)

func main() {
	http.HandleFunc("GET /orders", func(w http.ResponseWriter, r *http.Request) {
		if r.Header.Get("Authorization") == "" {
			http.Error(w, "login required", http.StatusUnauthorized)
			return
		}
		fmt.Fprintln(w, "3 orders")
	})

	// The public server serves the default mux, so it also serves pprof.
	app := httptest.NewServer(http.DefaultServeMux) // listens on 127.0.0.1
	defer app.Close()

	for _, path := range []string{"/orders", "/debug/pprof/", "/debug/pprof/cmdline"} {
		resp, err := http.Get(app.URL + path)
		if err != nil {
			fmt.Println(err)
			return
		}
		resp.Body.Close()
		fmt.Println("GET", path, resp.StatusCode)
	}
}
```

```output for=pprof
GET /orders 401
GET /debug/pprof/ 200
GET /debug/pprof/cmdline 200
```

The blank import of `net/http/pprof` adds its handlers to `http.DefaultServeMux`. The application serves that mux on its public port, so the profiles and the command line of the process are open to any caller. The route `/orders` has a check, and the debug routes have none.

```go run id=separate-mux
package main

import (
	"crypto/rand"
	"crypto/subtle"
	"encoding/hex"
	"fmt"
	"net/http"
	"net/http/httptest"
	"net/http/pprof"
)

var token = newToken() // made at start; the operators get it

func newToken() string {
	b := make([]byte, 16)
	rand.Read(b)
	return hex.EncodeToString(b)
}

// requireAuth runs next only for a request with the token.
func requireAuth(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		got := []byte(r.Header.Get("Authorization"))
		want := []byte("Bearer " + token)
		if subtle.ConstantTimeCompare(got, want) != 1 {
			http.Error(w, "login required", http.StatusUnauthorized)
			return
		}
		next.ServeHTTP(w, r)
	})
}

func main() {
	// The public server has its own mux. It never serves http.DefaultServeMux.
	public := http.NewServeMux()
	public.Handle("GET /orders", requireAuth(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		fmt.Fprintln(w, "3 orders")
	})))

	// The debug handlers go on a second mux, by hand, behind the same check.
	debug := http.NewServeMux()
	debug.Handle("GET /debug/pprof/", requireAuth(http.HandlerFunc(pprof.Index)))
	debug.Handle("GET /debug/pprof/cmdline", requireAuth(http.HandlerFunc(pprof.Cmdline)))

	app := httptest.NewServer(public)
	defer app.Close()
	ops := httptest.NewServer(debug) // a second listener on 127.0.0.1
	defer ops.Close()

	get := func(name, base, path, auth string) {
		req, _ := http.NewRequest("GET", base+path, nil)
		if auth != "" {
			req.Header.Set("Authorization", auth)
		}
		resp, err := http.DefaultClient.Do(req)
		if err != nil {
			fmt.Println(err)
			return
		}
		resp.Body.Close()
		fmt.Println(name, "GET", path, resp.StatusCode)
	}
	get("app  ", app.URL, "/orders", "")
	get("app  ", app.URL, "/debug/pprof/", "")
	get("debug", ops.URL, "/debug/pprof/cmdline", "")
	get("debug", ops.URL, "/debug/pprof/cmdline", "Bearer "+token)
}
```

```output for=separate-mux
app   GET /orders 401
app   GET /debug/pprof/ 404
debug GET /debug/pprof/cmdline 401
debug GET /debug/pprof/cmdline 200
```

The public server uses its own `http.ServeMux`, so a request for `/debug/pprof/` gets 404 there. The debug server registers `pprof.Index` and `pprof.Cmdline` by hand, and `requireAuth` wraps each one.

- Verdict: stdlib. `net/http` has the servers and the mux, `net/http/httptest` runs them on 127.0.0.1, and `crypto/subtle.ConstantTimeCompare` compares the token.
- The import of `net/http/pprof` still adds its handlers to `http.DefaultServeMux` when the package starts. The fix is safe because no server serves the default mux. A call such as `http.ListenAndServe(addr, nil)` serves the default mux, so it would bring the handlers back.
- The pprof documentation says that a program that does not use `DefaultServeMux` must register the handlers with the mux that it uses. `pprof.Index` serves the named profiles, such as `/debug/pprof/heap`. `Cmdline`, `Profile`, `Symbol` and `Trace` each need their own route.
- In a real deployment, bind the debug listener to a loopback address, such as `127.0.0.1:6060`, as a second layer. Keep the check as well, because other processes on the same machine can reach the loopback address.
