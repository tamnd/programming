```go run id=special-case-and-simple-fold
package main

import (
	"fmt"
	"strings"
	"unicode"
)

func main() {
	// strings.ToUpper maps each rune to one rune, so "ß" stays "ß".
	s := "straße"
	fmt.Println(strings.ToUpper(s), len([]rune(s)), len([]rune(strings.ToUpper(s))))

	// unicode.TurkishCase gives the Turkish rules for i and I.
	fmt.Println(strings.ToUpper("istanbul"), strings.ToUpperSpecial(unicode.TurkishCase, "istanbul"))
	fmt.Println(strings.ToLower("TITLE"), strings.ToLowerSpecial(unicode.TurkishCase, "TITLE"))

	// strings.ToLower has no context rule, so a final sigma stays "σ".
	fmt.Println(strings.ToLower("ΌΣΟΣ"))

	// EqualFold uses simple case folding: the sigmas match, "ß" and "SS" do not.
	fmt.Println(strings.EqualFold("ΌΣΟΣ", "όσος"), strings.EqualFold(s, "STRASSE"))

	// SimpleFold walks the orbit of runes that fold together: K, k and the Kelvin sign.
	r := 'K'
	var orbit []string
	for i := 0; i < 3; i++ {
		r = unicode.SimpleFold(r)
		orbit = append(orbit, fmt.Sprintf("%U", r))
	}
	fmt.Println(strings.Join(orbit, " "))

	// ToTitle and ToUpper differ for the digraph "ǳ".
	fmt.Println(string(unicode.ToUpper('ǳ')), string(unicode.ToTitle('ǳ')))
}
```

```output for=special-case-and-simple-fold
STRAßE 6 6
ISTANBUL İSTANBUL
title tıtle
όσοσ
true false
U+006B U+212A U+004B
Ǳ ǲ
```

- Verdict: stdlib. `strings.ToUpper` maps each rune to one rune, so `ß` stays `ß`. `ToUpperSpecial` and `ToLowerSpecial` with `unicode.TurkishCase` give the Turkish forms. `strings.ToLower` has no final sigma rule, so the result ends in `σ`. `EqualFold` matches the sigma forms but not `ß` and `SS`. `unicode.SimpleFold` goes from `K` to `k`, then to the Kelvin sign U+212A, and back to `K`. `unicode.ToTitle` gives `ǲ`.
