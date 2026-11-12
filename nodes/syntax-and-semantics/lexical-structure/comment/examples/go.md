```go run id=doc-comments-and-directives
package main

import (
	"fmt"
	"go/ast"
	"go/parser"
	"go/token"
	"strings"
)

const src = `package geo

// Area returns the area of a circle of radius r.
// A doc comment is the comment right before a declaration.
func Area(r float64) float64 {
	return 3.14159 * r * r // pi to 5 places
}

//go:noinline
func helper() {}
`

func main() {
	fset := token.NewFileSet()
	// Without parser.ParseComments, the parser drops all comments.
	f, err := parser.ParseFile(fset, "geo.go", src, parser.ParseComments)
	if err != nil {
		panic(err)
	}
	for _, d := range f.Decls {
		fn := d.(*ast.FuncDecl)
		fmt.Printf("%s: %q\n", fn.Name.Name, strings.TrimSpace(fn.Doc.Text()))
	}
	// Doc.Text drops directives such as //go:noinline, but they are still in the comment list.
	fmt.Println(len(f.Comments), "comment groups;", f.Comments[2].List[0].Text)
}
```

```output for=doc-comments-and-directives
Area: "Area returns the area of a circle of radius r.\nA doc comment is the comment right before a declaration."
helper: ""
3 comment groups; //go:noinline
```

- Verdict: native. Go has `//` and `/* */` comments. A doc comment is the comment right before a top-level declaration, and `go/parser` keeps comments only with the `ParseComments` flag. `Doc.Text` leaves out a directive such as `//go:noinline`, but the directive is still in the comment list of the file, and the toolchain reads it there.
