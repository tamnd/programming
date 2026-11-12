```go run id=byte-strings-and-runes
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	// A string is a read-only sequence of bytes. len counts the bytes.
	s := "naïve 😀"
	fmt.Println(len(s), utf8.RuneCountInString(s))

	// s[i] is a byte, and range decodes UTF-8 into runes.
	fmt.Printf("%T %d\n", s[2], s[2])
	var parts []string
	for i, r := range "aï😀" {
		parts = append(parts, fmt.Sprintf("%d:%c", i, r))
	}
	fmt.Println(strings.Join(parts, " "))

	// A string cannot change. Convert to []byte, which is a copy.
	b := []byte("cat")
	b[0] = 'b'
	fmt.Println(string(b))

	// A string can hold bytes that are not valid UTF-8.
	bad := "a\xffb"
	fmt.Println(len(bad), utf8.ValidString(bad), utf8.ValidString(s))

	// strings.Builder builds a string from parts.
	var sb strings.Builder
	for i := range 3 {
		fmt.Fprintf(&sb, "%d;", i)
	}
	fmt.Println(sb.String())
}
```

```output for=byte-strings-and-runes
11 7
uint8 195
0:a 1:ï 3:😀
bat
3 false true
0;1;2;
```

- Verdict: native. `len` counts the 11 bytes, and `utf8.RuneCountInString` counts 7 runes. `s[2]` is a byte (`uint8`), the first byte of `ï`. `range` decodes UTF-8, so the indexes jump from 1 to 3. A conversion to `[]byte` copies the bytes, and the copy can change. A string can hold the byte `0xFF`, which is not valid UTF-8. `strings.Builder` builds a string from parts.
