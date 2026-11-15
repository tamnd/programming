```go run id=go-stream
package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
)

func stream(path string) (count, total, largest int, err error) {
	f, err := os.Open(path)
	if err != nil {
		return 0, 0, 0, err
	}
	defer f.Close()
	sc := bufio.NewScanner(f)
	for sc.Scan() { // one line at a time, with no LF
		n, err := strconv.Atoi(sc.Text())
		if err != nil {
			return 0, 0, 0, err
		}
		if count == 0 || n > largest {
			largest = n
		}
		count++
		total += n
	}
	return count, total, largest, sc.Err()
}

func main() {
	dir, err := os.MkdirTemp("", "stream")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "numbers.txt")
	f, err := os.Create(path)
	if err != nil {
		panic(err)
	}
	w := bufio.NewWriter(f)
	for i := range 1_000_000 {
		fmt.Fprintf(w, "%d\n", i%1000)
	}
	if err := w.Flush(); err != nil {
		panic(err)
	}
	f.Close()

	count, total, largest, err := stream(path)
	fmt.Println("lines:", count, "sum:", total, "max:", largest, "error:", err)
}
```

```output
lines: 1000000 sum: 499500000 max: 999 error: <nil>
```

`bufio.NewScanner` reads the file in blocks and gives one line at a time, with no LF, through `Text`. The loop keeps three numbers, and `sc.Err()` after the loop shows whether the scan stopped on an error. A `bufio.Writer` writes the file, and `Flush` writes the last block.
