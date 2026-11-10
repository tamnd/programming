```go run id=from-environment
package main

import (
	"errors"
	"fmt"
	"os"
	"strings"
)

type Config struct {
	User     string
	Password string
}

// String keeps the password out of every log line that prints a Config.
func (c Config) String() string {
	return fmt.Sprintf("user=%s password=<set>", c.User)
}

// loadConfig reads the settings through lookup, which is os.LookupEnv in production.
func loadConfig(lookup func(string) (string, bool)) (Config, error) {
	password, ok := lookup("EXAMPLE_DB_PASSWORD")
	if !ok || strings.TrimSpace(password) == "" {
		return Config{}, errors.New("EXAMPLE_DB_PASSWORD is not set")
	}
	user, ok := lookup("EXAMPLE_DB_USER")
	if !ok {
		user = "app"
	}
	return Config{User: user, Password: password}, nil
}

func main() {
	if _, err := loadConfig(os.LookupEnv); err != nil {
		fmt.Println("refused to start:", err)
	}

	deployment := map[string]string{"EXAMPLE_DB_PASSWORD": "example-not-a-real-password"}
	cfg, err := loadConfig(func(name string) (string, bool) {
		v, ok := deployment[name]
		return v, ok
	})
	if err != nil {
		fmt.Println(err)
		return
	}
	fmt.Println(cfg)
	fmt.Printf("%+v\n", cfg)
}
```

```output for=from-environment
refused to start: EXAMPLE_DB_PASSWORD is not set
user=app password=<set>
user=app password=<set>
```

`loadConfig` takes the lookup function, so production passes `os.LookupEnv` and a test passes a map. The `String` method decides what a log line shows.

- Verdict: stdlib. `os.LookupEnv` tells an unset variable from an empty one, and `os.Getenv` returns the empty string for both. The task [[read-an-environment-variable]] shows both.
- `fmt` uses the `String` method for `%v`, `%+v` and `%s`, but `%#v` prints the Go syntax of the struct with every field. Do not log configuration with `%#v`.
- A string constant in Go code is in the binary as plain bytes. `go build -ldflags "-X main.password=..."` also puts the value in the binary, so it is not a way to keep a secret out.
- `//go:embed` copies a file into the binary at build time. Do not embed a file that holds a secret.
- gosec rule G101 looks for hard-coded credentials in names and values.
