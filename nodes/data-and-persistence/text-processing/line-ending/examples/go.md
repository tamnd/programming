```go run id=scan-lines
package main

import (
	"bufio"
	"fmt"
	"strings"
)

func main() {
	text := "one\ntwo\r\nthree\rfour"

	// ScanLines removes an LF or a CRLF. A CR alone does not end a line.
	s := bufio.NewScanner(strings.NewReader(text))
	var lines []string
	for s.Scan() {
		lines = append(lines, s.Text())
	}
	fmt.Printf("%q\n", lines)

	// strings.Split keeps the CR of a CRLF.
	fmt.Printf("%q\n", strings.Split(text, "\n"))

	// The last line has no LF, but the Scanner still returns it.
	s = bufio.NewScanner(strings.NewReader("a\nb"))
	n := 0
	for s.Scan() {
		n++
	}
	fmt.Println("lines in \"a\\nb\":", n, "LF count:", strings.Count("a\nb", "\n"))
}
```

```output for=scan-lines
["one" "two" "three\rfour"]
["one" "two\r" "three\rfour"]
lines in "a\nb": 2 LF count: 1
```

- Verdict: stdlib. `bufio.ScanLines`, the default split function of a `Scanner`, removes an LF or a CRLF, but a CR alone does not end a line. The Scanner returns the last line also when it has no LF, so it counts one line more than the LF characters.
