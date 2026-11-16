```go run id=child
package main

import (
	"errors"
	"fmt"
	"os"
	"os/exec"
)

const script = `echo "child sees GREETING=$GREETING"; GREETING=changed; echo "child set GREETING=$GREETING"; exit 3`

func main() {
	greeting := "hello"
	cmd := exec.Command("sh", "-c", script)
	cmd.Env = append(os.Environ(), "GREETING="+greeting)
	out, err := cmd.Output()
	var exitErr *exec.ExitError
	if err != nil && !errors.As(err, &exitErr) {
		fmt.Println("start failed:", err)
		return
	}
	fmt.Print(string(out))
	fmt.Println("child exit status:", cmd.ProcessState.ExitCode())
	fmt.Println("child pid differs:", cmd.Process.Pid != os.Getpid())
	fmt.Println("parent greeting:", greeting)
}
```

```output for=child
child sees GREETING=hello
child set GREETING=changed
child exit status: 3
child pid differs: true
parent greeting: hello
```

- Verdict: stdlib. `exec.Command` builds a `Cmd`; `Output` starts it, reads its standard output and waits for it.
- A non-zero exit status comes back as an `*exec.ExitError`. `ProcessState.ExitCode()` gives the number.
- `cmd.Env` is the whole environment of the child. The code appends `GREETING` to a copy of the environment of the parent.
- `os/exec` does not start a shell, so the example names `sh` itself.
