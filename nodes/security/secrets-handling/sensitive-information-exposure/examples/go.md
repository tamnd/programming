```go wrong id=log-struct
package main

import (
	"errors"
	"fmt"
	"io"
	"log/slog"
	"net/http"
	"net/http/httptest"
	"os"
)

type Config struct {
	Host     string
	User     string
	Password string
}

// Placeholders. A real service reads these from a secret store.
var cfg = Config{Host: "db.internal:5432", User: "shop", Password: "PLACEHOLDER-PASSWORD"}

func connect(c Config) error {
	return fmt.Errorf("connect %s:%s@%s: %w", c.User, c.Password, c.Host, errors.New("connection refused"))
}

func main() {
	// Text logs to standard output, without the time, so the output is stable.
	log := slog.New(slog.NewTextHandler(os.Stdout, &slog.HandlerOptions{
		ReplaceAttr: func(groups []string, a slog.Attr) slog.Attr {
			if a.Key == slog.TimeKey {
				return slog.Attr{}
			}
			return a
		},
	}))

	log.Info("starting", "config", cfg) // the whole struct goes to the log

	h := http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if err := connect(cfg); err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError) // the client gets the error
			return
		}
	})
	srv := httptest.NewServer(h)
	defer srv.Close()

	resp, err := http.Get(srv.URL + "/orders/7")
	if err != nil {
		fmt.Println(err)
		return
	}
	body, _ := io.ReadAll(resp.Body)
	resp.Body.Close()
	fmt.Print("client gets: ", resp.StatusCode, " ", string(body))
}
```

```output for=log-struct
level=INFO msg=starting config="{Host:db.internal:5432 User:shop Password:PLACEHOLDER-PASSWORD}"
client gets: 500 connect shop:PLACEHOLDER-PASSWORD@db.internal:5432: connection refused
```

The log line prints the whole `Config`, with the password. The handler sends `err.Error()` to the client, and the error text has the password too.

```go run id=log-valuer
package main

import (
	"crypto/rand"
	"encoding/hex"
	"errors"
	"fmt"
	"io"
	"log/slog"
	"net/http"
	"net/http/httptest"
	"os"
)

// Secret hides its value from fmt and from log/slog.
type Secret string

func (Secret) String() string        { return "REDACTED" }
func (Secret) LogValue() slog.Value { return slog.StringValue("REDACTED") }

type Config struct {
	Host     string
	User     string
	Password Secret
}

// Placeholders. A real service reads these from a secret store.
var cfg = Config{Host: "db.internal:5432", User: "shop", Password: "PLACEHOLDER-PASSWORD"}

func connect(c Config) error {
	return fmt.Errorf("connect %s:%s@%s: %w", c.User, c.Password, c.Host, errors.New("connection refused"))
}

func incidentID() string {
	b := make([]byte, 4)
	rand.Read(b)
	return hex.EncodeToString(b)
}

func main() {
	log := slog.New(slog.NewTextHandler(os.Stdout, &slog.HandlerOptions{
		ReplaceAttr: func(groups []string, a slog.Attr) slog.Attr {
			if a.Key == slog.TimeKey || a.Key == "incident" {
				return slog.Attr{} // dropped only to keep the demo output stable
			}
			return a
		},
	}))

	log.Info("starting", "host", cfg.Host, "user", cfg.User, "password", cfg.Password)

	h := http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if err := connect(cfg); err != nil {
			id := incidentID()
			log.Error("get order", "incident", id, "err", err) // details stay on the server
			w.Header().Set("Incident", id)
			http.Error(w, "internal error", http.StatusInternalServerError)
			return
		}
	})
	srv := httptest.NewServer(h)
	defer srv.Close()

	resp, err := http.Get(srv.URL + "/orders/7")
	if err != nil {
		fmt.Println(err)
		return
	}
	body, _ := io.ReadAll(resp.Body)
	resp.Body.Close()
	fmt.Print("client gets: ", resp.StatusCode, " ", string(body))
	fmt.Println("incident id sent:", len(resp.Header.Get("Incident")) == 8)
}
```

```output for=log-valuer
level=INFO msg=starting host=db.internal:5432 user=shop password=REDACTED
level=ERROR msg="get order" err="connect shop:REDACTED@db.internal:5432: connection refused"
client gets: 500 internal error
incident id sent: true
```

`Secret` implements `fmt.Stringer` and `slog.LogValuer`, so it prints as `REDACTED` in the error text and in the log. The handler sends a generic message and puts a random incident id in a header. The log keeps the error, with the same id.

- Verdict: stdlib. `log/slog` calls `LogValue` for a value that implements `LogValuer`. The documentation of `log/slog` names redaction of passwords as a use, and its example named Secret gives a token type a `LogValue` method that returns a fixed string.
- `fmt` calls `String()` for `%s` and `%v`, so the error built with `fmt.Errorf` shows `REDACTED` as well. A conversion back to `string`, such as `string(cfg.Password)`, removes the methods, so the value prints in full.
- `http.Error` writes the text that it gets to the client. Pass it a fixed message, not `err.Error()`.
- The demo removes the time and the incident id from the log only to keep its output the same on every run.
