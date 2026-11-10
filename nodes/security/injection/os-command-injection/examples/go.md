```go wrong id=sh-c
package main

import (
	"fmt"
	"os/exec"
)

func lookUp(name string) {
	out, _ := exec.Command("sh", "-c", "echo looking up "+name).Output()
	fmt.Print(string(out))
}

func main() {
	lookUp("report.txt")
	lookUp("report.txt; echo INJECTED")
}
```

```output for=sh-c
looking up report.txt
looking up report.txt
INJECTED
```

```go run id=args
package main

import (
	"fmt"
	"os/exec"
)

func lookUp(name string) {
	out, err := exec.Command("echo", "looking up", name).Output()
	if err != nil {
		fmt.Println("error:", err)
		return
	}
	fmt.Print(string(out))
}

func main() {
	lookUp("report.txt")
	lookUp("report.txt; echo INJECTED")
}
```

```output for=args
looking up report.txt
looking up report.txt; echo INJECTED
```

- Verdict: stdlib. `exec.Command` takes the program and each argument apart. The package documentation says that it intentionally does not invoke the system shell, so it does not expand globs, pipes or redirections.
- The weakness comes back when a program passes `sh -c` or `cmd /c` with a string built from input, as in the first example.
- `exec.Command` looks up the program in `PATH`. When the name of the program comes from input, the input selects the command. Choose the program from a fixed set in the code.
- gosec rule G204 reports `exec.Command` with arguments that are not constants, and G702 follows input to it. CodeQL query `go/command-injection` does the same.
