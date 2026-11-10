```go wrong id=join
package main

import (
	"fmt"
	"os"
	"path/filepath"
)

func main() {
	base, _ := os.MkdirTemp("", "traversal")
	defer os.RemoveAll(base)
	public := filepath.Join(base, "public")
	os.Mkdir(public, 0o755)
	os.WriteFile(filepath.Join(public, "report.txt"), []byte("quarterly report"), 0o644)
	os.WriteFile(filepath.Join(base, "secret.txt"), []byte("the secret key"), 0o644)

	readFile := func(name string) string {
		data, err := os.ReadFile(filepath.Join(public, name))
		if err != nil {
			return "error: " + err.Error()
		}
		return string(data)
	}

	fmt.Println(readFile("report.txt"))
	fmt.Println(readFile("../secret.txt"))
	fmt.Println(filepath.Join("public", "/etc/hosts"))
}
```

```output for=join
quarterly report
the secret key
public/etc/hosts
```

```go run id=root since=1.24
package main

import (
	"fmt"
	"io"
	"os"
	"path/filepath"
)

func main() {
	base, _ := os.MkdirTemp("", "traversal")
	defer os.RemoveAll(base)
	public := filepath.Join(base, "public")
	os.Mkdir(public, 0o755)
	os.WriteFile(filepath.Join(public, "report.txt"), []byte("quarterly report"), 0o644)
	os.WriteFile(filepath.Join(base, "secret.txt"), []byte("the secret key"), 0o644)
	os.Symlink("../secret.txt", filepath.Join(public, "link.txt"))

	root, err := os.OpenRoot(public)
	if err != nil {
		panic(err)
	}
	defer root.Close()

	readFile := func(name string) string {
		f, err := root.Open(name)
		if err != nil {
			return "error: " + err.Error()
		}
		defer f.Close()
		data, _ := io.ReadAll(f)
		return string(data)
	}

	fmt.Println(readFile("report.txt"))
	fmt.Println(readFile("../secret.txt"))
	fmt.Println(readFile("link.txt"))
	fmt.Println(filepath.IsLocal("report.txt"), filepath.IsLocal("../secret.txt"), filepath.IsLocal("/etc/hosts"))
}
```

```output for=root
quarterly report
error: openat ../secret.txt: path escapes from parent
error: openat link.txt: path escapes from parent
true false false
```

- Verdict: stdlib. `os.Root` opens a directory once. Its methods accept only names that stay beneath the directory, and return an error for a name with `..` that leaves it, or for a symbolic link that points out of it.
- On most platforms, `OpenRoot` opens a file descriptor of the directory, and the methods work relative to it. A separate check of the name before an open can race with another process that changes the directory. The documentation of `os.Root` lists its limits: it does not stop the crossing of file system boundaries or bind mounts, and on some platforms it does not stop such races.
- `filepath.Join` cleans the result, so `public/../secret.txt` becomes `secret.txt` next to `public`. Unlike the join functions of Python, Java, .NET and Rust, it does not drop the earlier segments for an absolute segment.
- `filepath.IsLocal` checks a name by its text alone. It does not look at symbolic links, so use it as a first check and `os.Root` for the open.
- gosec rule G304 reports file paths that come from input, G305 reports archive members written with no check, and G703 follows input to file operations. CodeQL queries `go/path-injection` and `go/zipslip` do the same.
