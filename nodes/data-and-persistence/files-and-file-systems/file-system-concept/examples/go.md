```go run id=io-fs-interface
package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"testing/fstest"
)

// count works on any fs.FS: a directory on the disk or a tree in memory.
func count(fsys fs.FS) (files, dirs int) {
	fs.WalkDir(fsys, ".", func(path string, d fs.DirEntry, err error) error {
		if err != nil {
			return err
		}
		if d.IsDir() {
			dirs++
		} else {
			files++
		}
		return nil
	})
	return files, dirs
}

func main() {
	os.MkdirAll("tree/src", 0o755)
	os.WriteFile("tree/src/main.txt", []byte("main\n"), 0o644)
	os.WriteFile("tree/readme.txt", []byte("read me\n"), 0o644)

	// os.DirFS gives the directory tree as an fs.FS.
	f, d := count(os.DirFS("tree"))
	fmt.Println("disk:", f, "files,", d, "directories")

	// fstest.MapFS is a file system in memory with the same interface.
	mem := fstest.MapFS{
		"a.txt":     {Data: []byte("a")},
		"sub/b.txt": {Data: []byte("b")},
		"sub/c.txt": {Data: []byte("c")},
	}
	f, d = count(mem)
	fmt.Println("memory:", f, "files,", d, "directories")

	// os.SameFile compares the device and the inode of two names.
	os.Link("tree/src/main.txt", "tree/main-link.txt")
	a, _ := os.Stat("tree/src/main.txt")
	b, _ := os.Stat("tree/main-link.txt")
	fmt.Println("same file:", os.SameFile(a, b))

	// A missing name gives an error that matches fs.ErrNotExist.
	_, err := os.Stat("tree/missing.txt")
	fmt.Println("not exist:", errors.Is(err, fs.ErrNotExist))
}
```

```output for=io-fs-interface
disk: 2 files, 2 directories
memory: 3 files, 2 directories
same file: true
not exist: true
```

- Verdict: stdlib. The `io/fs` interfaces separate the code from the storage. `os.DirFS` gives a directory on the disk as an `fs.FS`, and `fstest.MapFS` is a tree in memory, so one function with `fs.WalkDir` counts both. `os.SameFile` compares two `FileInfo` values, and a missing file gives an error that matches `fs.ErrNotExist`.
