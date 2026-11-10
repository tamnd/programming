```go run id=expr
package main

import "fmt"

// Expr is a closed set by convention: only the types in this package have the method.
type Expr interface{ isExpr() }

type Num struct{ Value int }
type Add struct{ Left, Right Expr }
type Mul struct{ Left, Right Expr }

func (Num) isExpr() {}
func (Add) isExpr() {}
func (Mul) isExpr() {}

func show(e Expr) string {
	switch e := e.(type) {
	case Num:
		return fmt.Sprint(e.Value)
	case Add:
		return "(" + show(e.Left) + " + " + show(e.Right) + ")"
	case Mul:
		return "(" + show(e.Left) + " * " + show(e.Right) + ")"
	default:
		panic(fmt.Sprintf("unknown node %T", e))
	}
}

func evaluate(e Expr) int {
	switch e := e.(type) {
	case Num:
		return e.Value
	case Add:
		return evaluate(e.Left) + evaluate(e.Right)
	case Mul:
		return evaluate(e.Left) * evaluate(e.Right)
	default:
		panic(fmt.Sprintf("unknown node %T", e))
	}
}

func count(e Expr) int {
	switch e := e.(type) {
	case Num:
		return 1
	case Add:
		return 1 + count(e.Left) + count(e.Right)
	case Mul:
		return 1 + count(e.Left) + count(e.Right)
	default:
		panic(fmt.Sprintf("unknown node %T", e))
	}
}

func main() {
	tree := Mul{Add{Num{2}, Num{3}}, Num{4}}
	fmt.Println("show:", show(tree))
	fmt.Println("evaluate:", evaluate(tree))
	fmt.Println("nodes:", count(tree))
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: idiom. A type switch picks the case by the dynamic type of the interface value. Each operation is one function with a case for each node type.
- The unexported method `isExpr` keeps other packages from adding node types. The compiler still does not check that a type switch covers every type, so each switch has a `default` that panics.
- `go/ast` has a `Visitor` interface and a `Walk` function. `Walk` holds the traversal, and the visitor's `Visit` method returns the visitor to use for the children, or `nil` to skip them.
