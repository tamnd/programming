```go run id=give-it-a-type
package main

import (
	"fmt"
	"strings"
)

// A named type is the lightest form: no wrapper struct, but its own
// methods, and the compiler stops a plain UserID from being passed as an Email.
type Email string

type UserID string

func ParseEmail(s string) (Email, error) {
	v := strings.ToLower(strings.TrimSpace(s))
	at := strings.IndexByte(v, '@')
	if at <= 0 || at != strings.LastIndexByte(v, '@') || at == len(v)-1 {
		return "", fmt.Errorf("not an email: %q", s)
	}
	return Email(v), nil
}

func (e Email) Domain() string { return string(e[strings.IndexByte(string(e), '@')+1:]) }

func notify(to Email) string { return "mail to " + string(to) }

func main() {
	a, _ := ParseEmail(" Ana@Example.org")
	b, _ := ParseEmail("ana@example.org")
	fmt.Println(a == b, a.Domain(), notify(a)) // == compares the values
	var id UserID = "u-17"
	_ = id // notify(id) does not compile: cannot use id (UserID) as Email
	if _, err := ParseEmail("not-an-email"); err != nil {
		fmt.Println(err)
	}
	fmt.Println("replace primitive with object: tests pass")
}
```

```output
true example.org mail to ana@example.org
not an email: "not-an-email"
replace primitive with object: tests pass
```
