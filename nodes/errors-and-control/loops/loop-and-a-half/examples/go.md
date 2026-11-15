```go run id=readstring-until-eof-and-scanner
package main

import (
	"bufio"
	"fmt"
	"io"
	"strconv"
	"strings"
)

func main() {
	// for with the test in the middle: read, test, process.
	r := bufio.NewReader(strings.NewReader("one\ntwo\n"))
	for {
		line, err := r.ReadString('\n')
		if err == io.EOF {
			break
		}
		if err != nil {
			fmt.Println("error:", err)
			return
		}
		fmt.Println("line:", strings.TrimSuffix(line, "\n"))
	}

	// Scan reads and reports success, so the read is the condition.
	sc := bufio.NewScanner(strings.NewReader("3 4 5"))
	sc.Split(bufio.ScanWords)
	sum := 0
	for sc.Scan() {
		n, err := strconv.Atoi(sc.Text())
		if err == nil {
			sum += n
		}
	}
	fmt.Println("sum:", sum)
}
```

```output for=readstring-until-eof-and-scanner
line: one
line: two
sum: 12
```

- Verdict: idiom. Go writes the middle exit as an endless `for` with `break`. `ReadString` returns `io.EOF` with an empty string after the last newline, but if the input ended without a newline, it would return the last line and `io.EOF` together, and this loop would drop that line. `Scanner` hides the problem: `Scan` reads the next token and returns `false` at the end.
