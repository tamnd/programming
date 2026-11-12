```go run id=go-parser-and-semicolons
package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
)

func main() {
	// go/parser builds the syntax tree of Go code.
	expr, err := parser.ParseExpr("price * (1 + rate)")
	if err != nil {
		panic(err)
	}
	bin := expr.(*ast.BinaryExpr)
	fmt.Printf("%T %s %T\n", bin.X, bin.Op, bin.Y)

	// ast.Inspect walks the tree.
	n := 0
	ast.Inspect(expr, func(node ast.Node) bool {
		if _, ok := node.(*ast.Ident); ok {
			n++
		}
		return true
	})
	fmt.Println("identifiers:", n)

	// A syntax error has a position.
	fset := token.NewFileSet()
	_, err = parser.ParseFile(fset, "t.go", "package p\nfunc f() { x := }", 0)
	fmt.Println(err)

	// Go inserts semicolons at the end of lines, so a { must be on the same line.
	_, err = parser.ParseFile(fset, "u.go", "package p\nfunc g()\n{\n}", 0)
	fmt.Println(err)
}
```

```output for=go-parser-and-semicolons
*ast.Ident * *ast.ParenExpr
identifiers: 2
t.go:2:17: expected operand, found '}'
u.go:3:1: unexpected semicolon or newline before {
```

- Verdict: native. `go/parser` gives the syntax tree of an expression, and `ast.Inspect` walks it. A syntax error has a file, a line and a column. Go inserts a semicolon after `func g()` at the end of the line, so a `{` on the next line is a syntax error.
