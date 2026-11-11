```go run id=bytes-and-runes
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	s := "cafe\u0301 \U0001F44D\U0001F3FD"
	fmt.Println(len(s), utf8.RuneCountInString(s))

	var found []string
	for i, r := range s { // range decodes UTF-8 into runes
		if r > 127 {
			found = append(found, fmt.Sprintf("%d:%U", i, r))
		}
	}
	fmt.Println(strings.Join(found, " "))

	b := []byte(s)
	fmt.Println(b[4], b[5], utf8.RuneLen('\u0301'))

	bad := "caf\xe9" // a Latin-1 byte, not valid UTF-8
	r, size := utf8.DecodeRuneInString(bad[3:])
	fmt.Println(utf8.ValidString(bad), r == utf8.RuneError, size)
}
```

```output for=bytes-and-runes
15 8
4:U+0301 7:U+1F44D 11:U+1F3FD
204 129 2
false true 1
```

```go wrong id=truncate-mid-rune
package main

import (
	"fmt"
	"unicode/utf8"
)

func truncate(s string, n int) string {
	if len(s) <= n {
		return s
	}
	return s[:n] // cuts at a byte, not at a character
}

func main() {
	name := "Ng\u00f4 B\u1ea3o"
	short := truncate(name, 3)
	fmt.Println(len(short), utf8.ValidString(short))
	fmt.Printf("%q\n", short)
}
```

```output for=truncate-mid-rune
3 false
"Ng\xc3"
```

- Verdict: native. A Go string holds bytes, which are UTF-8 by convention, so `len` counts bytes: 15 for the sample. A `rune` is a code point, `range` over a string decodes one rune at a time and gives its byte offset, and `unicode/utf8` counts, checks and decodes runes. An invalid byte decodes to `utf8.RuneError`, U+FFFD, with a size of 1. Grapheme clusters need a package outside the standard library.
- The wrong block truncates a Vietnamese name to 3 bytes. The third byte is the first half of the two bytes of `ô`, so the result is not valid UTF-8 and prints as a broken character. Truncate at a rune boundary, with `utf8.RuneStart` or by counting runes.
