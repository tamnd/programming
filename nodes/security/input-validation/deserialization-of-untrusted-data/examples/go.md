```go wrong id=bind-account
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

type Account struct {
	Name    string `json:"name"`
	Theme   string `json:"theme"`
	IsAdmin bool   `json:"isAdmin"`
}

func main() {
	body := `{"name": "mallory", "theme": "dark", "isAdmin": true}`
	var account Account
	if err := json.NewDecoder(strings.NewReader(body)).Decode(&account); err != nil {
		fmt.Println("error:", err)
		return
	}
	fmt.Printf("%s: theme %s, admin %t\n", account.Name, account.Theme, account.IsAdmin)
}
```

```output for=bind-account
mallory: theme dark, admin true
```

`encoding/json` fills only the type that the code passes, so the bytes cannot choose a type. The request still sets `IsAdmin`, because the stored type has that field.

```go run id=strict-request
package main

import (
	"encoding/json"
	"errors"
	"fmt"
	"strings"
)

// Only the fields that this operation may change.
type SettingsRequest struct {
	Theme string `json:"theme"`
}

func parse(body string) (SettingsRequest, error) {
	var request SettingsRequest
	decoder := json.NewDecoder(strings.NewReader(body))
	decoder.DisallowUnknownFields()
	if err := decoder.Decode(&request); err != nil {
		return SettingsRequest{}, err
	}
	if request.Theme != "dark" && request.Theme != "light" {
		return SettingsRequest{}, errors.New("unknown theme")
	}
	return request, nil
}

func main() {
	for _, body := range []string{`{"theme": "dark"}`, `{"theme": "dark", "isAdmin": true}`} {
		request, err := parse(body)
		if err != nil {
			fmt.Println("refused:", err)
			continue
		}
		fmt.Println("theme", request.Theme)
	}
}
```

```output for=strict-request
theme dark
refused: json: unknown field "isAdmin"
```

`DisallowUnknownFields` makes the decoder return an error for a key that matches no field of the struct.

- Verdict: stdlib. `encoding/json`, `encoding/xml` and `encoding/gob` decode into types that the program passes. The bytes do not name a class to create.
- Decode into a request type with only the needed fields, call `DisallowUnknownFields`, and validate the values.
- Decoding into `any` gives maps and slices, not program types. Check each value before use.
