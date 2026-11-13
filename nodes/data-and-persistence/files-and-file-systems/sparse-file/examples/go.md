```go run id=seek-and-truncate
package main

import (
	"fmt"
	"io"
	"os"
	"syscall"
)

// allocated gives the space of the file on the disk. Blocks counts 512-byte units.
func allocated(path string) int64 {
	info, err := os.Stat(path)
	if err != nil {
		panic(err)
	}
	return info.Sys().(*syscall.Stat_t).Blocks * 512
}

func main() {
	dir, err := os.MkdirTemp("", "sparse")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	path := dir + "/sparse.bin"

	f, err := os.Create(path)
	if err != nil {
		panic(err)
	}
	// A seek past the end and a write leave a hole before the data.
	if _, err := f.Seek(64<<20, io.SeekStart); err != nil {
		panic(err)
	}
	if _, err := f.Write([]byte("end")); err != nil {
		panic(err)
	}
	// Truncate to a larger size adds a hole at the end.
	if err := f.Truncate(128 << 20); err != nil {
		panic(err)
	}
	f.Close()

	info, _ := os.Stat(path)
	fmt.Println("size:           ", info.Size())
	fmt.Println("uses less space:", allocated(path) < info.Size())

	data, _ := os.ReadFile(path)
	fmt.Printf("bytes 0..3:      %q\n", data[:3])
	fmt.Printf("bytes at 64 MiB: %q\n", data[64<<20:64<<20+3])
}
```

```output for=seek-and-truncate
size:            134217728
uses less space: true
bytes 0..3:      "\x00\x00\x00"
bytes at 64 MiB: "end"
```

- Verdict: stdlib. `File.Seek` past the end and `File.Truncate` to a larger size make holes. The allocated size is not in `fs.FileInfo`, so the example reads `Blocks` from `syscall.Stat_t`, which exists only on Unix. `SEEK_DATA` and `SEEK_HOLE` are not constants of the `io` package.
