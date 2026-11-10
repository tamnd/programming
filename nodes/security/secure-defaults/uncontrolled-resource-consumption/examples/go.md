```go wrong id=read-all
package main

import (
	"fmt"
	"io"
	"net/http"
	"net/http/httptest"
	"strings"
)

// The handler reads the whole body, however large it is.
func upload(w http.ResponseWriter, r *http.Request) {
	body, err := io.ReadAll(r.Body)
	if err != nil {
		http.Error(w, "read error", http.StatusBadRequest)
		return
	}
	fmt.Fprintf(w, "stored %d bytes", len(body))
}

func main() {
	srv := httptest.NewServer(http.HandlerFunc(upload))
	defer srv.Close()

	// 3 MiB stands for a body of any size.
	resp, err := http.Post(srv.URL, "text/plain", strings.NewReader(strings.Repeat("x", 3<<20)))
	if err != nil {
		fmt.Println(err)
		return
	}
	defer resp.Body.Close()
	text, _ := io.ReadAll(resp.Body)
	fmt.Println(resp.StatusCode, string(text))
}
```

```output for=read-all
200 stored 3145728 bytes
```

`io.ReadAll` reads until the end of the body, however large it is. This is the Go example in CWE-400.

```go run id=max-bytes
package main

import (
	"errors"
	"fmt"
	"io"
	"net/http"
	"net/http/httptest"
	"strings"
	"time"
)

const maxBody = 1 << 20 // 1 MiB

// MaxBytesReader stops the read after maxBody bytes and returns *http.MaxBytesError.
func upload(w http.ResponseWriter, r *http.Request) {
	r.Body = http.MaxBytesReader(w, r.Body, maxBody)
	body, err := io.ReadAll(r.Body)
	var tooLarge *http.MaxBytesError
	if errors.As(err, &tooLarge) {
		http.Error(w, fmt.Sprintf("body larger than %d bytes", tooLarge.Limit), http.StatusRequestEntityTooLarge)
		return
	}
	if err != nil {
		http.Error(w, "read error", http.StatusBadRequest)
		return
	}
	fmt.Fprintf(w, "stored %d bytes", len(body))
}

func post(url string, size int) {
	resp, err := http.Post(url, "text/plain", strings.NewReader(strings.Repeat("x", size)))
	if err != nil {
		fmt.Println(err)
		return
	}
	defer resp.Body.Close()
	text, _ := io.ReadAll(resp.Body)
	fmt.Println(resp.StatusCode, strings.TrimSpace(string(text)))
}

func main() {
	srv := httptest.NewUnstartedServer(http.HandlerFunc(upload))
	// Limits on time and on header size. The zero value of each timeout means no limit.
	srv.Config.ReadHeaderTimeout = 5 * time.Second
	srv.Config.ReadTimeout = 30 * time.Second
	srv.Config.WriteTimeout = 30 * time.Second
	srv.Config.IdleTimeout = 60 * time.Second
	srv.Config.MaxHeaderBytes = 16 << 10
	srv.Start()
	defer srv.Close()

	post(srv.URL, 3<<20)
	post(srv.URL, 1000)
}
```

```output for=max-bytes
413 body larger than 1048576 bytes
200 stored 1000 bytes
```

`http.MaxBytesReader` returns an error after the limit, and the error is a `*http.MaxBytesError` with the limit in it. The server also has limits on time and on the size of the headers.

```go run id=linear-regexp
package main

import (
	"fmt"
	"regexp"
	"strings"
	"time"
)

func main() {
	// The nested quantifier from the OWASP ReDoS page. Go's regexp package does not backtrack.
	evil := regexp.MustCompile(`^(a+)+$`)

	for _, n := range []int{30, 10_000, 1_000_000} {
		input := strings.Repeat("a", n) + "!"
		start := time.Now()
		matched := evil.MatchString(input)
		fast := time.Since(start) < time.Second
		fmt.Printf("%d letters: matched=%v, under one second=%v\n", n, matched, fast)
	}
}
```

```output for=linear-regexp
30 letters: matched=false, under one second=true
10000 letters: matched=false, under one second=true
1000000 letters: matched=false, under one second=true
```

The pattern that backtracks for a long time in Python answers at once in Go, even with a million letters.

- Verdict: stdlib. `net/http` has `MaxBytesReader`, `MaxBytesHandler` and the timeout fields of `Server`. `regexp` runs in linear time.
- `MaxBytesReader` also tells the `ResponseWriter` to close the connection after the limit, so the client cannot keep sending.
- When `ReadTimeout`, `ReadHeaderTimeout`, `WriteTimeout` and `IdleTimeout` are all zero, the server has no timeouts. `http.ListenAndServe` makes a `Server` with only an address and a handler, so create your own `Server` and set the fields.
- For a deadline on the work of one request, pass `r.Context()` to the functions that do the work, and wrap it with `context.WithTimeout`.
