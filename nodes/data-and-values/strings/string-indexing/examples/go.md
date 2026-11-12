```go run id=byte-index-and-range
package main

import (
	"fmt"
	"unicode/utf8"
)

func main() {
	s := "naïve 😀"

	// s[i] is a byte. The letter ï is the two bytes c3 af.
	fmt.Println(len(s), s[2], s[3])
	fmt.Printf("%x %x\n", s[2], s[3])

	// range gives the byte index where each rune starts.
	for i, r := range s {
		if r > 127 {
			fmt.Printf("%c at byte %d\n", r, i)
		}
	}

	// DecodeRuneInString reads the rune at a byte index.
	r, size := utf8.DecodeRuneInString(s[7:])
	fmt.Printf("%c %d\n", r, size)

	// A slice in the middle of a rune gives invalid UTF-8.
	fmt.Println(utf8.ValidString(s[:3]), utf8.RuneStart(s[3]))

	// []rune(s) allows an index by code point, after an O(n) copy.
	runes := []rune(s)
	fmt.Println(len(runes), string(runes[6]))
}
```

```output for=byte-index-and-range
11 195 175
c3 af
ï at byte 2
😀 at byte 7
😀 4
false false
7 😀
```

- Verdict: native. `s[2]` and `s[3]` are the two bytes `c3 af` of `ï`, and `len(s)` is 11. `range` gives the byte index of each rune, so the emoji is at byte 7. `utf8.DecodeRuneInString` reads the 4-byte rune there. `s[:3]` is not valid UTF-8, and byte 3 is not the start of a rune. `[]rune(s)` gives 7 runes that a program can index.
