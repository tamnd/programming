```go run id=escape
package main

import "fmt"

type session struct {
	user     string
	loggedIn bool
}

// openSession returns the address of a local variable.
func openSession(user string) *session {
	s := session{user: user, loggedIn: true}
	return &s
}

func main() {
	s := openSession("alice")
	fmt.Println(s.user, "logged in:", s.loggedIn)
}
```

```output for=escape
alice logged in: true
```

In C, this function would return a dangling pointer to its stack frame. In Go, each variable exists as long as there are references to it. When the compiler cannot prove that a local variable is not used after the function returns, it puts the variable on the garbage-collected heap.

```go wrong id=closed-file
package main

import (
	"errors"
	"fmt"
	"os"
)

func main() {
	f, err := os.CreateTemp("", "session")
	if err != nil {
		panic(err)
	}
	defer os.Remove(f.Name())
	f.Close()

	_, err = f.WriteString("alice\n")
	fmt.Println("write after close:", err != nil, errors.Is(err, os.ErrClosed))
}
```

```output for=closed-file
write after close: true true
```

- Verdict: native. The garbage collector frees memory only when no reference to it is left, so a pointer cannot point to freed memory.
- The same mistake still happens with resources that the program closes by hand: files, connections, buffers from a pool. The memory is safe, and the operation returns an error such as `os.ErrClosed`. Check the error, and use `defer f.Close()` after a successful open so the close happens at the end of the function.
- The packages `unsafe` and `cgo` can hold pointers that the collector does not track. Memory that C code frees can still be used from Go.
