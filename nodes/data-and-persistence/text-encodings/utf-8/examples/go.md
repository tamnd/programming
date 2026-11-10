```go run id=runes
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	text := "nai\u0308ve \U0001F469\u200d\U0001F4BB"
	fmt.Println("bytes", len(text))
	fmt.Println("runes", utf8.RuneCountInString(text))
	fmt.Printf("byte 3 is %X, text[3:4] is %q\n", text[3], text[3:4])

	for index, r := range "a\u00e9\U0001D11E" {
		fmt.Printf("offset %d U+%04X %d bytes\n", index, r, utf8.RuneLen(r))
	}

	buffer := make([]byte, utf8.UTFMax)
	size := utf8.EncodeRune(buffer, 0x20AC)
	fmt.Printf("U+20AC % X\n", buffer[:size])
	size = utf8.EncodeRune(buffer, 0xD800)
	fmt.Printf("U+D800 % X\n", buffer[:size])
}
```

```output for=runes
bytes 19
runes 10
byte 3 is CC, text[3:4] is "\xcc"
offset 0 U+0061 1 bytes
offset 1 U+00E9 2 bytes
offset 3 U+1D11E 4 bytes
U+20AC E2 82 AC
U+D800 EF BF BD
```

```go run id=invalid
package main

import (
	"fmt"
	"strings"
	"unicode/utf8"
)

func main() {
	bad := "a\xc0\xaf\xed\xa0\x80b"
	fmt.Println(utf8.ValidString(bad), utf8.RuneCountInString(bad))

	for index, r := range bad {
		if r == utf8.RuneError {
			fmt.Print(index, " ")
		}
	}
	fmt.Println()

	fmt.Printf("%q\n", string([]rune(bad)))
	fmt.Printf("%q\n", strings.ToValidUTF8(bad, "\uFFFD"))
}
```

```output for=invalid
false 7
1 2 3 4 5
"a�����b"
"a�b"
```

- Verdict: stdlib. A Go string is a sequence of bytes, and Go source files are UTF-8. The package `unicode/utf8` encodes, decodes, counts and validates.
- `len` counts bytes. `utf8.RuneCountInString` counts code points. An index such as `text[2]` gives one byte, and a slice can cut a character in half.
- A `for range` loop over a string decodes one rune at a time and gives the byte offset of each rune. For an invalid byte it gives `utf8.RuneError` (U+FFFD) and moves one byte forward.
- `utf8.EncodeRune` writes U+FFFD for a surrogate or a value above U+10FFFF.
- The conversion to `[]rune` gives one U+FFFD for each bad byte. `strings.ToValidUTF8` replaces each run of bad bytes with one replacement string.
- The standard library has no normalization and no grapheme segmentation. The module `golang.org/x/text/unicode/norm` does normalization.
