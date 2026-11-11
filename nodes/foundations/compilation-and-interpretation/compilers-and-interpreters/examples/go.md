```go run id=parse-and-interpret
package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"strconv"
)

// eval interprets a Go expression tree: a compiled program running an interpreter.
func eval(e ast.Expr, env map[string]int) int {
	switch n := e.(type) {
	case *ast.BasicLit:
		v, _ := strconv.Atoi(n.Value)
		return v
	case *ast.Ident:
		return env[n.Name]
	case *ast.ParenExpr:
		return eval(n.X, env)
	case *ast.BinaryExpr:
		a, b := eval(n.X, env), eval(n.Y, env)
		switch n.Op {
		case token.ADD:
			return a + b
		case token.SUB:
			return a - b
		case token.MUL:
			return a * b
		}
	}
	panic("unsupported expression")
}

func main() {
	expr, err := parser.ParseExpr("(x*2 + 1) * x")
	if err != nil {
		panic(err)
	}
	fmt.Printf("%T\n", expr)
	for _, x := range []int{1, 5, 10} {
		fmt.Println(x, eval(expr, map[string]int{"x": x}))
	}

	_, err = parser.ParseExpr("x * (2 +")
	fmt.Println(err != nil)
}
```

```output for=parse-and-interpret
*ast.BinaryExpr
1 3
5 55
10 210
true
```

- Verdict: native. The Go toolchain compiles a program ahead of time to one machine-code executable, with the runtime and the garbage collector linked in. `go run` compiles to a temporary file and runs it, so it feels like an interpreter but is not one.
- The standard library includes the Go parser, `go/parser`, which turns source text into a syntax tree. The example walks that tree to evaluate an expression: an interpreter inside a compiled program. A syntax error comes back as an error value, not a crash.
