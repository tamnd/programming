```go run id=pprof
package main

import (
	"bytes"
	"compress/gzip"
	"fmt"
	"io"
	"runtime/pprof"
)

var calls = map[string]int{} // instrumentation: a counter in each function

func fib(n int) int {
	calls["fib"]++
	if n < 2 {
		return n
	}
	return fib(n-1) + fib(n-2)
}

func main() {
	var buf bytes.Buffer
	if err := pprof.StartCPUProfile(&buf); err != nil {
		fmt.Println("start:", err)
		return
	}
	fib(25)
	pprof.StopCPUProfile() // the profile is complete only after Stop

	// The profile is a gzip-compressed protocol buffer.
	zr, err := gzip.NewReader(&buf)
	if err != nil {
		fmt.Println("not gzip:", err)
		return
	}
	data, _ := io.ReadAll(zr)
	fmt.Println("profile is gzip:", true)
	fmt.Println("profile has data:", len(data) > 0)
	fmt.Println("fib calls:", calls["fib"])
}
```

```output for=pprof
profile is gzip: true
profile has data: true
fib calls: 242785
```

`StartCPUProfile` buffers the profile and writes it to the writer. The profile is complete only after `StopCPUProfile`. A program that returns without the call leaves an empty file.

```go wrong id=no-stop
package main

import (
	"bytes"
	"fmt"
	"runtime/pprof"
)

func fib(n int) int {
	if n < 2 {
		return n
	}
	return fib(n-1) + fib(n-2)
}

func main() {
	var buf bytes.Buffer
	pprof.StartCPUProfile(&buf)
	fib(25)
	fmt.Println("bytes before Stop:", buf.Len()) // the profile is still empty
}
```

```output for=no-stop
bytes before Stop: 0
```

- Verdict: stdlib. `runtime/pprof` samples the call stacks of the program 100 times a second and writes a gzip-compressed protocol buffer.
- The number of samples depends on timing, so the example prints only facts that do not change. The call counter is manual instrumentation.
- `go test -cpuprofile cpu.prof` profiles the tests and benchmarks of a package. `go tool pprof cpu.prof` reads the result.
- `net/http/pprof` serves the profiles of a running server over HTTP.
