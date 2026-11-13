```go run id=stat-lstat-sys
package main

import (
	"fmt"
	"os"
	"syscall"
	"time"
)

func main() {
	os.WriteFile("data.txt", []byte("hello"), 0o600)
	os.Chmod("data.txt", 0o640)
	t := time.Unix(1700000000, 0)
	os.Chtimes("data.txt", t, t)
	os.Link("data.txt", "second-name.txt")
	os.Symlink("data.txt", "link")

	// os.Stat follows the link, os.Lstat does not.
	st, _ := os.Stat("link")
	lst, _ := os.Lstat("link")
	fmt.Println("stat: ", st.Name(), st.Mode(), st.Size(), st.ModTime().Unix())
	fmt.Println("lstat:", lst.Name(), lst.Mode()&os.ModeSymlink != 0, lst.Size())

	// Sys gives the stat structure of the system on Unix.
	if sys, ok := st.Sys().(*syscall.Stat_t); ok {
		fmt.Println("links:", sys.Nlink)
	}
	other, _ := os.Stat("second-name.txt")
	fmt.Println("same file:", os.SameFile(st, other))
}
```

```output for=stat-lstat-sys
stat:  link -rw-r----- 5 1700000000
lstat: link true 8
links: 2
same file: true
```

- Verdict: stdlib. `os.Stat` and `os.Lstat` return an `fs.FileInfo` with `Name`, `Size`, `Mode`, `ModTime`, `IsDir` and `Sys`. `Name` is the name that the program gave, not the name of the target. The `FileMode` holds the type bits and the permission bits, and it prints as `ls` does. The other fields of the stat structure, such as the link count, come from `Sys`, which is a `*syscall.Stat_t` on Unix. `os.SameFile` compares two `FileInfo` values.
