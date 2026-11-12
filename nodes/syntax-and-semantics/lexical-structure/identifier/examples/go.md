```go run id=exported-names-and-blank
package main

import (
	"fmt"
	"go/token"
)

// Total is exported, because its name starts with an upper-case letter.
// total would be visible only inside this package.
func Total(xs ...int) int {
	t := 0
	for _, x := range xs { // _ is the blank identifier: the index is not used
		t += x
	}
	return t
}

func main() {
	π := 3.14159
	größe := 2.0
	fmt.Println(Total(1, 2, 3), π*größe)

	for _, s := range []string{"x1", "1x", "Total", "total", "_", "func", "größe", "a-b"} {
		fmt.Printf("%-6s identifier=%-5v keyword=%-5v exported=%v\n",
			s, token.IsIdentifier(s), token.IsKeyword(s), token.IsExported(s))
	}

	// A name is a letter followed by letters and digits. U+0301, the combining acute accent, is a mark,
	// so the decomposed spelling of café is not an identifier, and Go does not normalize it.
	nfc, nfd := "caf\u00e9", "cafe\u0301"
	fmt.Println(token.IsIdentifier(nfc), token.IsIdentifier(nfd), nfc == nfd)
}
```

```output for=exported-names-and-blank
6 6.28318
x1     identifier=true  keyword=false exported=false
1x     identifier=false keyword=false exported=false
Total  identifier=true  keyword=false exported=true
total  identifier=true  keyword=false exported=false
_      identifier=true  keyword=false exported=false
func   identifier=false keyword=true  exported=false
größe  identifier=true  keyword=false exported=false
a-b    identifier=false keyword=false exported=false
true false false
```

- Verdict: native. A Go name is a letter followed by letters and digits, and `_` counts as a letter. A name that starts with an upper-case letter is exported from its package, so the case of the first letter sets the visibility. `_` is the blank identifier and binds nothing. `go/token` checks the form of a name, the keywords and export. The decomposed spelling of `café` contains a combining mark, so it is not an identifier.
