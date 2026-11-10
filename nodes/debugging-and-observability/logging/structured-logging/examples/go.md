```go run id=records
package main

import (
	"errors"
	"log/slog"
	"os"
)

// Password is a secret. Its LogValue hides it in every log record.
type Password string

func (Password) LogValue() slog.Value { return slog.StringValue("REDACTED") }

func main() {
	opts := &slog.HandlerOptions{
		Level: slog.LevelInfo,
		ReplaceAttr: func(groups []string, a slog.Attr) slog.Attr {
			if a.Key == slog.TimeKey && len(groups) == 0 {
				return slog.Attr{} // drop the time so that the output is fixed
			}
			return a
		},
	}
	log := slog.New(slog.NewJSONHandler(os.Stdout, opts))

	log.Debug("cart loaded", "items", 3) // below Info: dropped
	log.Info("order placed", "order", 1042, "total_cents", 1999)
	req := log.With("request", "r-7")
	req.Warn("stock low", slog.String("sku", "A-17"), slog.Int("left", 2))
	req.Error("price failed", "order", 1043, "err", errors.New("division by zero"))
	req.Info("login", "user", "bob", "password", Password("hunter2"))

	text := slog.New(slog.NewTextHandler(os.Stdout, opts))
	text.Info("order placed", "order", 1042, "note", "gift wrap")
}
```

```output for=records
{"level":"INFO","msg":"order placed","order":1042,"total_cents":1999}
{"level":"WARN","msg":"stock low","request":"r-7","sku":"A-17","left":2}
{"level":"ERROR","msg":"price failed","request":"r-7","order":1043,"err":"division by zero"}
{"level":"INFO","msg":"login","request":"r-7","user":"bob","password":"REDACTED"}
level=INFO msg="order placed" order=1042 note="gift wrap"
```

A log call takes pairs of a key and a value. A missing key shifts the pairs, and slog writes the value under the key `!BADKEY`. `go vet` reports these calls.

```go wrong id=badkey
package main

import (
	"log/slog"
	"os"
)

func main() {
	opts := &slog.HandlerOptions{
		ReplaceAttr: func(groups []string, a slog.Attr) slog.Attr {
			if a.Key == slog.TimeKey && len(groups) == 0 {
				return slog.Attr{}
			}
			return a
		},
	}
	log := slog.New(slog.NewJSONHandler(os.Stdout, opts))
	order, total := 1042, 1999
	log.Info("order placed", "order", order, total) // the key "total_cents" is missing
	log.Info("order placed", order, total)          // no keys at all
}
```

```output for=badkey
{"level":"INFO","msg":"order placed","order":1042,"!BADKEY":1999}
{"level":"INFO","msg":"order placed","!BADKEY":1042,"!BADKEY":1999}
```

- Verdict: stdlib. `log/slog` writes records with a time, a level, a message and key-value attributes.
- `JSONHandler` writes JSON and `TextHandler` writes `key=value` pairs. `ReplaceAttr` can change or remove any attribute, here the time.
- `Logger.With` adds attributes to every record of a new logger, such as the id of a request.
- A type with a `LogValue` method controls how it appears in a record, so a password type can always print `REDACTED`.
