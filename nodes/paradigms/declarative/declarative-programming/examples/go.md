```go run id=template
package main

import (
	"fmt"
	"os"
	"regexp"
	"text/template"
)

type Order struct {
	Item       string
	Qty, Price int
}

func main() {
	t := template.Must(template.New("report").Parse(
		"{{range .}}{{if gt .Qty 1}}{{.Item}} x{{.Qty}}\n{{end}}{{end}}"))
	orders := []Order{{"pen", 3, 2}, {"book", 1, 12}, {"ink", 2, 5}}
	if err := t.Execute(os.Stdout, orders); err != nil {
		panic(err)
	}

	re := regexp.MustCompile(`\b(\d{4})-(\d{2})-(\d{2})\b`)
	for _, m := range re.FindAllStringSubmatch("from 2024-01-31 to 2025-12-01", -1) {
		fmt.Println(m[1], m[2], m[3])
	}
}
```

```output for=template
pen x3
ink x2
2024 01 31
2025 12 01
```

- Verdict: idiom. Go code is mostly imperative, but the standard library has declarative parts: `text/template` and `html/template` describe output, `regexp` describes text, and struct tags describe how `encoding/json` maps fields.
- The `regexp` package guarantees time linear in the size of the input, because it does not support backreferences.
