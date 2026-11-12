```go run id=rule-subset-and-byte-limit
package main

import (
	"fmt"
	"strings"
	"unicode"
	"unicode/utf8"
)

const zwj = '\u200d'

// isExtend covers nonspacing and enclosing marks and the emoji skin tone modifiers.
func isExtend(r rune) bool {
	return unicode.In(r, unicode.Mn, unicode.Me) || (r >= 0x1F3FB && r <= 0x1F3FF)
}

// graphemes applies a small subset of the UAX 29 rules: no break before
// Extend or ZWJ, no break after ZWJ, and regional indicators in pairs.
// A real program uses a library such as github.com/rivo/uniseg.
func graphemes(s string) []string {
	var out []string
	var prev rune = -1
	riRun := 0
	for _, r := range s {
		ri := unicode.Is(unicode.Regional_Indicator, r)
		join := prev >= 0 && (isExtend(r) || r == zwj || prev == zwj)
		if prev >= 0 && ri && riRun%2 == 1 {
			join = true
		}
		if join {
			out[len(out)-1] += string(r)
		} else {
			out = append(out, string(r))
		}
		if ri {
			riRun++
		} else {
			riRun = 0
		}
		prev = r
	}
	return out
}

func main() {
	t := "cafe\u0301 \U0001F1EB\U0001F1F7 \U0001F469\U0001F3FD\u200d\U0001F692"

	// len counts bytes, and RuneCountInString counts code points.
	fmt.Println(len(t), utf8.RuneCountInString(t))

	g := graphemes(t)
	fmt.Println(len(g))
	sizes := make([]string, len(g))
	for i, x := range g {
		sizes[i] = fmt.Sprint(len(x))
	}
	fmt.Println(strings.Join(sizes, " "))

	// A cut at a byte limit can end inside the emoji. Cut at a grapheme boundary instead.
	limit, n := 22, 0
	for _, x := range g {
		if n+len(x) > limit {
			break
		}
		n += len(x)
	}
	fmt.Printf("%v %d %q\n", utf8.ValidString(t[:limit]), n, t[:n])
}
```

```output for=rule-subset-and-byte-limit
31 13
8
1 1 1 3 1 8 1 15
false 16 "café 🇫🇷 "
```

- Verdict: library. Go has no grapheme package in its standard library, so this sketch uses `unicode.Mn`, `unicode.Me` and `unicode.Regional_Indicator` with a small subset of the rules. A real program uses `github.com/rivo/uniseg`. The accented `e` takes 3 bytes, the flag 8 and the emoji 15. A cut at byte 22 is not valid UTF-8, so the code keeps only whole clusters and stops at byte 16.
