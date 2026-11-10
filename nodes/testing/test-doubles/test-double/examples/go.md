```go run id=register
package main

import (
	"fmt"
	"io/fs"
	"strings"
	"testing/fstest"
)

type Users interface {
	Exists(email string) bool
	Add(email string)
}

type Mailer interface {
	Send(to, body string)
}

func Register(email string, users Users, mailer Mailer) string {
	if users.Exists(email) {
		return "taken"
	}
	users.Add(email)
	mailer.Send(email, "Welcome!")
	return "created"
}

// fakeUsers satisfies Users without naming it: Go interfaces are implicit.
type fakeUsers map[string]bool

func (f fakeUsers) Exists(email string) bool { return f[email] }
func (f fakeUsers) Add(email string)          { f[email] = true }

// spyMailer records each call.
type spyMailer struct{ sent []string }

func (s *spyMailer) Send(to, body string) { s.sent = append(s.sent, to+": "+body) }

// CountLines reads a file through fs.FS, so a test can pass a fake file system.
func CountLines(fsys fs.FS, name string) (int, error) {
	data, err := fs.ReadFile(fsys, name)
	if err != nil {
		return 0, err
	}
	return strings.Count(string(data), "\n"), nil
}

func main() {
	users := fakeUsers{}
	mailer := &spyMailer{}
	fmt.Println(Register("ada@example.com", users, mailer))
	fmt.Println(Register("ada@example.com", users, mailer))
	fmt.Println(mailer.sent)

	fsys := fstest.MapFS{"notes.txt": {Data: []byte("one\ntwo\n")}}
	fmt.Println(CountLines(fsys, "notes.txt"))
	fmt.Println(CountLines(fsys, "missing.txt"))
}
```

```output
created
taken
[ada@example.com: Welcome!]
2 <nil>
0 open missing.txt: file does not exist
```

- Verdict: idiom. Go has no mock package. A type satisfies an interface when it has the methods, with no `implements` clause, so a double is any small type with those methods.
- The fake store is a map type with two methods. The spy needs a pointer receiver, because `Send` changes the slice.
- The standard library has fakes for tests. `testing/fstest.MapFS` is a file system in memory, and `CountLines` accepts it because it takes an `fs.FS`. `net/http/httptest` gives `ResponseRecorder` and a test server.
- The second call to `CountLines` shows that the fake also gives the real error for a missing file.
