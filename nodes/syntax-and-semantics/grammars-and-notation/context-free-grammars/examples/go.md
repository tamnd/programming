```go run id=check-a-grammar
package main

import (
	"fmt"
	"regexp"
	"sort"
	"strings"
)

// A grammar in the EBNF style of the Go specification.
const grammar = `
Expr    = Term { ( "+" | "-" ) Term } .
Term    = Factor { ( "*" | "/" ) Factor } .
Factor  = number | "(" Expr ")" | Call .
Call    = name "(" [ ArgList ] ")" .
Args    = Expr { "," Expr } .
Unused  = "?" .
`

func main() {
	defined := map[string]bool{}
	used := map[string]bool{"Expr": true} // Expr is the start symbol
	symbol := regexp.MustCompile(`"[^"]*"|[A-Za-z_]\w*`)
	for _, line := range strings.Split(strings.TrimSpace(grammar), "\n") {
		lhs, rhs, _ := strings.Cut(line, "=")
		defined[strings.TrimSpace(lhs)] = true
		for _, s := range symbol.FindAllString(rhs, -1) {
			if s[0] >= 'A' && s[0] <= 'Z' { // CamelCase names are nonterminals
				used[s] = true
			}
		}
	}
	var undefined, unused []string
	for s := range used {
		if !defined[s] {
			undefined = append(undefined, s)
		}
	}
	for s := range defined {
		if !used[s] {
			unused = append(unused, s)
		}
	}
	sort.Strings(undefined)
	sort.Strings(unused)
	fmt.Println("productions:", len(defined))
	fmt.Println("undefined:  ", undefined)
	fmt.Println("unused:     ", unused)
}
```

```output for=check-a-grammar
productions: 6
undefined:   [ArgList]
unused:      [Args Unused]
```

- Verdict: library. The `goyacc` tool in `golang.org/x/tools` generates LALR parsers, and `golang.org/x/exp/ebnf` reads and checks grammars in the notation of the Go specification. The example does a small part of that check by hand. It reads six productions, treats CamelCase names as nonterminals as the specification does, and reports `ArgList`, a name that the grammar uses but never defines, and two productions that the start symbol cannot reach. Real grammars collect such mistakes as they grow.
