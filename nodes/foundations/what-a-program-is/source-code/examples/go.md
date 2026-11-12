```go run id=format-and-parse-source
package main

import (
	"fmt"
	"go/ast"
	"go/format"
	"go/parser"
	"go/token"
)

func main() {
	src := "package p\nfunc area( w,h int )int{return w*h // product\n}\n"

	// gofmt rewrites the text into the one standard layout.
	out, err := format.Source([]byte(src))
	if err != nil {
		panic(err)
	}
	fmt.Print(string(out))

	// The parser gives a tree, and each node knows its position in the text.
	fset := token.NewFileSet()
	f, err := parser.ParseFile(fset, "area.go", src, parser.ParseComments)
	if err != nil {
		panic(err)
	}
	ast.Inspect(f, func(n ast.Node) bool {
		if id, ok := n.(*ast.Ident); ok {
			fmt.Printf("%s at %s\n", id.Name, fset.Position(id.Pos()))
		}
		return true
	})
	fmt.Println("comments:", len(f.Comments))
}
```

```output for=format-and-parse-source
package p

func area(w, h int) int {
	return w * h // product
}
p at area.go:1:9
area at area.go:2:6
w at area.go:2:12
h at area.go:2:14
int at area.go:2:16
int at area.go:2:21
w at area.go:2:32
h at area.go:2:34
comments: 1
```

- Verdict: native. The Go standard library contains the parser and the formatter of the language. `format.Source` prints messy source in the layout of `gofmt` and keeps the comment. `parser.ParseFile` builds the tree, and `fset.Position` turns the position of each identifier into a file, a line and a column, which is how Go tools report errors.
