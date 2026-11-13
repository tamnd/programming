```go run id=filemode-bits
package main

import (
	"fmt"
	"io/fs"
	"os"
	"syscall"
)

func main() {
	// The umask removes bits from the mode of each new file.
	old := syscall.Umask(0o027)
	os.WriteFile("new.txt", []byte("x"), 0o666)
	os.Mkdir("dir", 0o777)
	syscall.Umask(old)

	for _, name := range []string{"new.txt", "dir"} {
		st, _ := os.Stat(name)
		fmt.Printf("%-8s %v %04o\n", name, st.Mode(), st.Mode().Perm())
	}

	// Go has its own bits for setuid, setgid and sticky. os.Chmod maps them.
	os.Chmod("new.txt", 0o755|fs.ModeSetuid)
	os.Chmod("dir", 0o777|fs.ModeSticky)
	a, _ := os.Stat("new.txt")
	b, _ := os.Stat("dir")
	fmt.Println(a.Mode(), b.Mode())
	fmt.Println("setuid:", a.Mode()&fs.ModeSetuid != 0)
}
```

```output for=filemode-bits
new.txt  -rw-r----- 0640
dir      drwxr-x--- 0750
urwxr-xr-x dtrwxrwxrwx
setuid: true
```

- Verdict: stdlib. A `fs.FileMode` holds the nine permission bits in its low bits, and `Perm` keeps only them. Go does not use the Unix values for the setuid, setgid and sticky bits. It has its own bits, `ModeSetuid`, `ModeSetgid` and `ModeSticky`, which print as letters before the permissions, and `os.Chmod` maps them to the system bits. The umask is in the `syscall` package, which is not portable.
