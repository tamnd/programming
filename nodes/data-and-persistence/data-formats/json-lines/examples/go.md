```go run id=read-write
package main

import (
	"bufio"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

type Event struct {
	Level string `json:"level"`
	Note  string `json:"note"`
}

func main() {
	dir, _ := os.MkdirTemp("", "jsonl-")
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "events.jsonl")

	f, _ := os.Create(path)
	enc := json.NewEncoder(f) // Encode writes each value and a newline.
	enc.Encode(Event{"info", "short"})
	enc.Encode(Event{"warn", strings.Repeat("x", 70000)})
	f.Close()

	for _, max := range []int{bufio.MaxScanTokenSize, 1 << 20} {
		f, _ := os.Open(path)
		sc := bufio.NewScanner(f)
		sc.Buffer(make([]byte, 0, 4096), max)
		for sc.Scan() {
			var e Event
			if err := json.Unmarshal(sc.Bytes(), &e); err != nil {
				fmt.Println(err)
				break
			}
			fmt.Println(max, e.Level, len(e.Note))
		}
		fmt.Println(max, "error:", sc.Err())
		f.Close()
	}
}
```

```output for=read-write
65536 info 5
65536 error: bufio.Scanner: token too long
1048576 info 5
1048576 warn 70000
1048576 error: <nil>
```

- Verdict: stdlib. `json.Encoder` writes each value and a newline. `bufio.Scanner` reads one line at a time, and `json.Unmarshal` parses it. The second record is longer than 64 KiB, so with the default limit `Scan` stops and `Err` returns `bufio.Scanner: token too long`. With a limit of 1 MiB the scanner reads both records.
