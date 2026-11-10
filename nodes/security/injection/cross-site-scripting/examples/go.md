```go wrong id=text-template
package main

import (
	"os"
	"text/template"
)

const page = `<p><b>{{.Author}}</b>: {{.Text}}</p>
<a href="{{.Home}}">home page</a>
`

type comment struct {
	Author, Text, Home string
}

func main() {
	t := template.Must(template.New("page").Parse(page))
	t.Execute(os.Stdout, comment{"alice", "Nice post.", "https://example.com/alice"})
	t.Execute(os.Stdout, comment{"mallory", "<script>alert(1)</script>", "javascript:alert(1)"})
}
```

```output for=text-template
<p><b>alice</b>: Nice post.</p>
<a href="https://example.com/alice">home page</a>
<p><b>mallory</b>: <script>alert(1)</script></p>
<a href="javascript:alert(1)">home page</a>
```

```go run id=html-template
package main

import (
	"html/template"
	"os"
)

const page = `<p><b>{{.Author}}</b>: {{.Text}}</p>
<a href="{{.Home}}">home page</a>
`

type comment struct {
	Author, Text, Home string
}

func main() {
	t := template.Must(template.New("page").Parse(page))
	t.Execute(os.Stdout, comment{"alice", "Nice post.", "https://example.com/alice"})
	t.Execute(os.Stdout, comment{"mallory", "<script>alert(1)</script>", "javascript:alert(1)"})
}
```

```output for=html-template
<p><b>alice</b>: Nice post.</p>
<a href="https://example.com/alice">home page</a>
<p><b>mallory</b>: &lt;script&gt;alert(1)&lt;/script&gt;</p>
<a href="#ZgotmplZ">home page</a>
```

- Verdict: stdlib. `html/template` has the same API as `text/template`, and the only change between the two programs is the import path.
- `html/template` escapes by context. It parses the template, finds that `{{.Text}}` is in HTML text and that `{{.Home}}` is in a URL attribute, and picks the escape for each place. It replaces a URL with an unsafe scheme by `#ZgotmplZ`.
- `text/template` does no escaping. It is correct for plain text output and wrong for HTML.
- The package trusts the template author and does not trust the data. The types `template.HTML`, `template.JS` and `template.URL` mark a value as safe and turn the escape off for it. Never convert input to these types. gosec rule G203 reports such conversions.
- gosec rule G705 and CodeQL query `go/reflected-xss` follow input from a request to the response writer.
