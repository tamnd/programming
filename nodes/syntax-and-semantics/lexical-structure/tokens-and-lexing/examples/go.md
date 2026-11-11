```go run id=go-scanner
package main

import (
	"fmt"
	"go/scanner"
	"go/token"
)

func main() {
	src := []byte("x := 0b1010 + 1_000\nif x > 2 {\n\tx++\n}\n")
	fset := token.NewFileSet()
	file := fset.AddFile("x.go", -1, len(src))
	var s scanner.Scanner
	s.Init(file, src, nil, 0)
	for {
		_, tok, lit := s.Scan()
		if tok == token.EOF {
			break
		}
		if tok == token.SEMICOLON && lit == "\n" {
			lit = "inserted at a newline"
		}
		fmt.Printf("%-6s %q\n", tok, lit)
	}
	fmt.Println(0b1010+1_000, 0o17, 'A', len(`C:\new`))
}
```

```output for=go-scanner
IDENT  "x"
:=     ""
INT    "0b1010"
+      ""
INT    "1_000"
;      "inserted at a newline"
if     "if"
IDENT  "x"
>      ""
INT    "2"
{      ""
IDENT  "x"
++     ""
;      "inserted at a newline"
}      ""
;      "inserted at a newline"
1010 15 65 6
```

```go fails expect=compile id=brace-on-next-line
package main

import "fmt"

func main() {
	x := 3
	if x > 2
	{ // a semicolon was inserted after the 2
		fmt.Println("big")
	}
}
```

- Verdict: stdlib. The `go/scanner` and `go/token` packages of the standard library are the lexer that Go tools use. The output shows the semicolons that the lexer inserts at the end of a line that ends with a name, a literal, `++` or a closing brace. Go has binary, octal and hex literals with underscores, rune literals that are numbers, and raw strings in backquotes.
- The fails block puts the opening brace of an `if` on the next line. The lexer inserts a semicolon after `2`, so the parser sees `if x > 2;` with no block and rejects it. This is why Go style puts the brace on the same line.
