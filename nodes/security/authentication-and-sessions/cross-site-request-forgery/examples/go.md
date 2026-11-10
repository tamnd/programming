```go run id=token
package main

import (
	"crypto/rand"
	"crypto/subtle"
	"encoding/hex"
	"fmt"
)

type session struct {
	user, csrf string
}

func newToken() string {
	b := make([]byte, 32)
	rand.Read(b) // crypto/rand, not math/rand
	return hex.EncodeToString(b)
}

var sessions = map[string]*session{"c0ffee": {user: "alice", csrf: newToken()}}

func changeEmail(cookie, formToken, email string) string {
	s, ok := sessions[cookie]
	if !ok {
		return "401 not logged in"
	}
	if subtle.ConstantTimeCompare([]byte(formToken), []byte(s.csrf)) != 1 {
		return "403 missing or wrong CSRF token"
	}
	return fmt.Sprintf("200 email of %s is %s", s.user, email)
}

func main() {
	fmt.Println(changeEmail("c0ffee", sessions["c0ffee"].csrf, "alice@example.net"))
	fmt.Println(changeEmail("c0ffee", "", "mallory@attacker.example"))
}
```

```output for=token
200 email of alice is alice@example.net
403 missing or wrong CSRF token
```

The token comes from `crypto/rand`, and `subtle.ConstantTimeCompare` compares it without a timing leak. The main example shows `http.CrossOriginProtection`, which needs no token.

- Verdict: stdlib. `net/http` has `CrossOriginProtection`, a check of `Sec-Fetch-Site` with an `Origin` fallback. It allows GET, HEAD and OPTIONS, so those methods must not change state.
- Wrap the whole mux with `CrossOriginProtection.Handler`, not single routes, so that a route added later is covered too.
- Use `AddTrustedOrigin` for the other origins that may post to the server. Keep `AddInsecureBypassPattern` for endpoints that must accept cross-origin requests and have another defence.
