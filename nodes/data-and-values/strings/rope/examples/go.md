```go run id=newline-counts-and-line-start
package main

import (
	"fmt"
	"strings"
)

// Each node also stores the number of newlines in its subtree, so a line lookup
// walks one path from the root.
type rope struct {
	s     string
	l, r  *rope
	n     int
	lines int
}

func leaf(s string) *rope { return &rope{s: s, n: len(s), lines: strings.Count(s, "\n")} }

func cat(a, b *rope) *rope { return &rope{l: a, r: b, n: a.n + b.n, lines: a.lines + b.lines} }

func text(t *rope) string {
	if t.l == nil {
		return t.s
	}
	return text(t.l) + text(t.r)
}

// lineStart returns the byte offset where line k starts. Line 0 starts at 0,
// and line k starts after the k-th newline.
func lineStart(t *rope, k int) int {
	if k == 0 {
		return 0
	}
	off := 0
	for t.l != nil {
		if k <= t.l.lines {
			t = t.l
		} else {
			k -= t.l.lines
			off += t.l.n
			t = t.r
		}
	}
	i := 0
	for ; k > 0; k-- {
		i += strings.IndexByte(t.s[i:], '\n') + 1
	}
	return off + i
}

func main() {
	t := cat(cat(leaf("one\ntwo\nthr"), leaf("ee\nfour\n")), leaf("five\nsix"))
	full := text(t)
	fmt.Println(t.n, t.lines)
	for k := 0; k <= t.lines; k++ {
		off := lineStart(t, k)
		line, _, _ := strings.Cut(full[off:], "\n")
		fmt.Println(k, off, line)
	}
}
```

```output for=newline-counts-and-line-start
27 5
0 0 one
1 4 two
2 8 three
3 14 four
4 19 five
5 24 six
```

- Verdict: idiom. Go has no rope in its standard library. Each node stores a count of newlines, so `lineStart` walks one path from the root to find the leaf that holds the k-th newline. The offsets are in bytes, because Go strings are bytes.
