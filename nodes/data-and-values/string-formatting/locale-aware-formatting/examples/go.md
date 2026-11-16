```go run id=no-locale-in-fmt
package main

import (
	"fmt"
	"strconv"
	"strings"
)

type locale struct {
	name, group, decimal string
}

func format(x float64, digits int, loc locale) string {
	text := strconv.FormatFloat(x, 'f', digits, 64)
	sign := ""
	if strings.HasPrefix(text, "-") {
		sign, text = "-", text[1:]
	}
	whole, frac, _ := strings.Cut(text, ".")
	var b strings.Builder
	for i := range len(whole) {
		if i > 0 && (len(whole)-i)%3 == 0 {
			b.WriteString(loc.group)
		}
		b.WriteByte(whole[i])
	}
	if frac != "" {
		b.WriteString(loc.decimal + frac)
	}
	return sign + b.String()
}

func main() {
	x := 1234567.891
	for _, loc := range []locale{{"en-US", ",", "."}, {"de-DE", ".", ","}, {"it-CH", "'", "."}} {
		fmt.Println(loc.name, format(x, 2, loc), format(-1234.5, 1, loc))
	}
	fmt.Printf("%.2f %v\n", x, x)
}
```

```output for=no-locale-in-fmt
en-US 1,234,567.89 -1,234.5
de-DE 1.234.567,89 -1.234,5
it-CH 1'234'567.89 -1'234.5
1234567.89 1.234567891e+06
```

- Verdict: library. The package `fmt` has no locale, so `%.2f` gives the same text on each computer. The module `golang.org/x/text/message` adds a `Printer` for a language tag with the CLDR data. Without it, a program puts in the symbols itself, as here, and it must know the group sizes of each locale.
