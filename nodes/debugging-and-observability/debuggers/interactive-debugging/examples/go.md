```go run id=stack
package main

import (
	"bytes"
	"fmt"
	"runtime"
	"runtime/debug"
	"strings"
)

// where returns the calls that led to its caller, one per line, without the
// file paths and addresses that change from one machine to another.
func where() []string {
	pcs := make([]uintptr, 16)
	n := runtime.Callers(2, pcs) // skip runtime.Callers and where
	frames := runtime.CallersFrames(pcs[:n])
	var lines []string
	for {
		frame, more := frames.Next()
		if strings.HasPrefix(frame.Function, "main.") {
			lines = append(lines, fmt.Sprintf("%s line %d", frame.Function, frame.Line))
		}
		if !more {
			return lines
		}
	}
}

func price(item string) int {
	if item == "bread" {
		for _, line := range where() {
			fmt.Println("  " + line)
		}
		return 250
	}
	return 30
}

func total(cart []string) int {
	sum := 0
	for _, item := range cart {
		sum += price(item)
	}
	return sum
}

func main() {
	fmt.Println("total:", total([]string{"apple", "bread"}))

	// debug.Stack formats the stack as a panic does: a goroutine header,
	// then a function and a file:line for each frame.
	stack := debug.Stack()
	header, _, _ := bytes.Cut(stack, []byte("\n"))
	fmt.Println(string(header))
	fmt.Println("mentions main.main:", bytes.Contains(stack, []byte("main.main()")))
}
```

```output for=stack
  main.price line 31
  main.total line 42
  main.main line 48
total: 280
goroutine 1 [running]:
mentions main.main: true
```

`runtime.CallersFrames` turns program counters into frames. It also gives a frame for each call that the compiler inlined. `runtime.FuncForPC` looks at one program counter and cannot do this.

```go norun reason=platform id=funcforpc
package main

import (
	"fmt"
	"runtime"
	"strings"
)

// names looks up each program counter with FuncForPC. The documentation of
// runtime.Callers advises against this: FuncForPC cannot see inlined calls.
func names() []string {
	pcs := make([]uintptr, 16)
	n := runtime.Callers(2, pcs)
	var out []string
	for _, pc := range pcs[:n] {
		name := runtime.FuncForPC(pc).Name()
		if strings.HasPrefix(name, "main.") {
			out = append(out, name)
		}
	}
	return out
}

func lookup(item string) []string { return names() } // small: the compiler inlines it

func checkout() []string { return lookup("bread") }

func main() {
	fmt.Println(strings.Join(checkout(), " <- "))
}
```

The compiler inlines `lookup` and `checkout` into `main`. For the program counter of an inlined call, the name that `FuncForPC` gives depends on the compiler and on the architecture. With one Go release, this program can print the right chain on one machine and a chain with a missing or repeated function on another. So the output is not shown here. The fix is `runtime.CallersFrames`, as in the first example.

- Verdict: library. Delve (`dlv`) is the debugger for Go, and it is not part of the Go distribution. The Go documentation says that GDB does not understand Go programs well.
- `runtime.Breakpoint()` executes a breakpoint trap. Under a debugger, the program stops there.
- `debug.Stack` returns the stack of the current goroutine as text. `debug.PrintStack` writes it to standard error.
- `go build -gcflags=all="-N -l"` turns off optimizations and inlining, so the debugger sees each variable and each call.
