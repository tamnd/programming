```go run id=trace
package main

import (
	"fmt"
	"runtime"
	"strings"
)

func where() string {
	pc := make([]uintptr, 16)
	n := runtime.Callers(2, pc) // skip Callers and where
	frames := runtime.CallersFrames(pc[:n])
	var names []string
	for {
		f, more := frames.Next()
		if name, ok := strings.CutPrefix(f.Function, "main."); ok {
			names = append(names, name)
		}
		if !more {
			break
		}
	}
	return strings.Join(names, " <- ")
}

func c() {
	fmt.Println("      stack:", where())
}

func b() {
	fmt.Println("    call c")
	c()
	fmt.Println("    back in b")
}

func a() {
	fmt.Println("  call b")
	b()
	fmt.Println("  back in a")
}

func main() {
	fmt.Println("call a")
	a()
	fmt.Println("back in main")
}
```

```output for=trace
call a
  call b
    call c
      stack: c <- b <- a <- main
    back in b
  back in a
back in main
```

- Verdict: native. Each goroutine has its own stack. `runtime.Callers` records the program counters on it, and `runtime.CallersFrames` turns them into function names.
- The compiler can inline `a`, `b` and `c`, but `CallersFrames` accounts for inlined functions, so the names are all there.
