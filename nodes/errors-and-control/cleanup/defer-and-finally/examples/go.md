```go run id=copy
package main

import (
	"errors"
	"fmt"
)

type file struct {
	name    string
	failure error
}

func open(name string) *file {
	fmt.Println("open", name)
	return &file{name: name}
}

func (f *file) Close() error {
	fmt.Println("close", f.name)
	return f.failure
}

func copyFile(src, dst string, closeFails bool) (err error) {
	in := open(src)
	defer in.Close()
	out := open(dst)
	if closeFails {
		out.failure = errors.New("disk full")
	}
	defer func() {
		if cerr := out.Close(); cerr != nil && err == nil {
			err = fmt.Errorf("close %s: %w", dst, cerr)
		}
	}()
	fmt.Println("copy", src, "to", dst)
	return nil
}

func main() {
	fmt.Println("result:", copyFile("a.txt", "b.txt", false))
	fmt.Println("result:", copyFile("a.txt", "c.txt", true))

	for i := 0; i <= 3; i++ {
		defer fmt.Print(i, " ")
	}
	fmt.Print("deferred: ")
}
```

```output
open a.txt
open b.txt
copy a.txt to b.txt
close b.txt
close a.txt
result: <nil>
open a.txt
open c.txt
copy a.txt to c.txt
close c.txt
close a.txt
result: close c.txt: disk full
deferred: 3 2 1 0
```

- Verdict: native. A `defer` statement schedules a call to run when the function returns, whether by `return`, by the end of the body or by a panic.
- The deferred calls run in reverse order. `out` closes before `in`, and the loop prints `3 2 1 0`.
- A deferred function literal can read and change the named result `err`. Here it reports the error from `Close`, which a plain `defer out.Close()` would discard.

A deferred call evaluates its arguments when the `defer` statement runs, not when the call runs. The program means to print the final count, but it prints the count at the time of the `defer`.

```go wrong id=arguments
package main

import "fmt"

func main() {
	count := 0
	defer fmt.Println("processed", count, "items")
	for i := 0; i < 3; i++ {
		count++
	}
	fmt.Println("count is", count)
}
```

```output
count is 3
processed 0 items
```

A `defer` inside a loop does not run at the end of each iteration. It runs when the function returns, so every file in the loop stays open until then.

```go wrong id=loop
package main

import "fmt"

var openFiles, maxOpen int

type file struct{ name string }

func open(name string) *file {
	openFiles++
	if openFiles > maxOpen {
		maxOpen = openFiles
	}
	return &file{name}
}

func (f *file) Close() { openFiles-- }

func processAll(names []string) {
	for _, name := range names {
		f := open(name)
		defer f.Close()
	}
	fmt.Println("open at the end of the loop:", openFiles)
}

func main() {
	names := make([]string, 1000)
	for i := range names {
		names[i] = fmt.Sprintf("log-%d.txt", i)
	}
	processAll(names)
	fmt.Println("most files open at once:", maxOpen)
}
```

```output
open at the end of the loop: 1000
most files open at once: 1000
```
