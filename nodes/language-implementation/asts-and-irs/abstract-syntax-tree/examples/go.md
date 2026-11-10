```go run id=parse-walk
package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"strconv"
	"strings"
)

func eval(e ast.Expr, env map[string]int) int {
	switch e := e.(type) {
	case *ast.BasicLit:
		v, _ := strconv.Atoi(e.Value)
		return v
	case *ast.Ident:
		return env[e.Name]
	case *ast.ParenExpr:
		return eval(e.X, env)
	case *ast.BinaryExpr:
		l, r := eval(e.X, env), eval(e.Y, env)
		switch e.Op {
		case token.ADD:
			return l + r
		case token.SUB:
			return l - r
		case token.MUL:
			return l * r
		}
	}
	panic(fmt.Sprintf("unsupported node %T", e))
}

func main() {
	expr, err := parser.ParseExpr("1 + 2*(3 - x)")
	if err != nil {
		panic(err)
	}

	depth, count := 0, 0
	var names []string
	ast.Inspect(expr, func(n ast.Node) bool {
		if n == nil {
			depth--
			return false
		}
		count++
		pad := strings.Repeat("  ", depth)
		switch n := n.(type) {
		case *ast.BinaryExpr:
			fmt.Printf("%sBinaryExpr %s\n", pad, n.Op)
		case *ast.ParenExpr:
			fmt.Printf("%sParenExpr\n", pad)
		case *ast.BasicLit:
			fmt.Printf("%sBasicLit %s\n", pad, n.Value)
		case *ast.Ident:
			fmt.Printf("%sIdent %s\n", pad, n.Name)
			names = append(names, n.Name)
		}
		depth++
		return true
	})
	fmt.Println("nodes:", count)
	fmt.Println("names:", names)
	fmt.Println("value at x = 1:", eval(expr, map[string]int{"x": 1}))
}
```

```output for=parse-walk
BinaryExpr +
  BasicLit 1
  BinaryExpr *
    BasicLit 2
    ParenExpr
      BinaryExpr -
        BasicLit 3
        Ident x
nodes: 8
names: [x]
value at x = 1: 5
```

- Verdict: stdlib. `go/parser` parses Go source into the node types of `go/ast`.
- The tree keeps the parentheses as an `*ast.ParenExpr` node, so it has eight nodes where Python's tree has seven. The evaluator must handle `ParenExpr` and step into its `X` field.
- `ast.Inspect` calls the function for each node in depth-first order, and calls it with `nil` after the children of a node. The example uses that call to track the depth.
- The same packages back `gofmt` and `go vet`. A tool that inspects Go code needs no third-party parser.
