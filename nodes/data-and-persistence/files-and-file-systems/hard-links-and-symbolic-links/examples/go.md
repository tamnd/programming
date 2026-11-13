```go run id=same-file-and-lstat
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"syscall"
)

func main() {
	if err := os.WriteFile("data.txt", []byte("alpha\n"), 0o644); err != nil {
		panic(err)
	}
	if err := os.Link("data.txt", "hard.txt"); err != nil {
		panic(err)
	}
	if err := os.Symlink("data.txt", "soft.txt"); err != nil {
		panic(err)
	}

	a, _ := os.Stat("data.txt")
	h, _ := os.Stat("hard.txt")
	l, _ := os.Lstat("soft.txt")
	fmt.Println("SameFile:   ", os.SameFile(a, h))
	fmt.Println("Nlink:      ", a.Sys().(*syscall.Stat_t).Nlink)
	fmt.Println("Lstat mode: ", l.Mode().Type() == fs.ModeSymlink)
	target, _ := os.Readlink("soft.txt")
	fmt.Println("Readlink:   ", target)

	// A link to a directory with the same name again: the system finds a loop.
	if err := os.Symlink("loop", "loop"); err != nil {
		panic(err)
	}
	_, err := os.Stat("loop")
	fmt.Println("loop:       ", errors.Is(err, syscall.ELOOP))
}
```

```output for=same-file-and-lstat
SameFile:    true
Nlink:       2
Lstat mode:  true
Readlink:    data.txt
loop:        true
```

- Verdict: stdlib. `os.Link`, `os.Symlink`, `os.Readlink` and `os.Lstat` are in the `os` package, and `os.SameFile` compares the device and the inode. The link count is not in `fs.FileInfo`, so the example reads `Nlink` from `syscall.Stat_t`, which exists only on Unix.
