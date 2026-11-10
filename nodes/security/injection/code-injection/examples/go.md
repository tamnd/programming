```go wrong id=user-template
package main

import (
	"fmt"
	"os"
	"text/template"
)

type Account struct {
	Name   string
	APIKey string
}

// greet parses the text that the user wrote as a template.
func greet(userText string, account Account) {
	t, err := template.New("greeting").Parse(userText)
	if err != nil {
		fmt.Println("error:", err)
		return
	}
	t.Execute(os.Stdout, account)
	fmt.Println()
}

func main() {
	account := Account{Name: "alice", APIKey: "placeholder-not-a-real-key"}
	greet("Hello, {{.Name}}!", account)
	greet("Hello, {{.APIKey}}!", account)
	greet("{{.}}", account)
}
```

```output for=user-template
Hello, alice!
Hello, placeholder-not-a-real-key!
{alice placeholder-not-a-real-key}
```

The user wrote the template, so the user chose which fields to print. `{{.}}` prints the whole value.

```go run id=fixed-template
package main

import (
	"os"
	"text/template"
)

// The template text is a constant in the code. Its author is trusted.
var greeting = template.Must(template.New("greeting").Parse("{{.Greeting}}, {{.Name}}!\n"))

// The view holds only the values that the template needs.
type view struct {
	Greeting string
	Name     string
}

func main() {
	for _, userText := range []string{"Hello", "Hello {{.APIKey}}"} {
		// The text from the user is data. The template prints it and never parses it.
		greeting.Execute(os.Stdout, view{Greeting: userText, Name: "alice"})
	}
}
```

```output for=fixed-template
Hello, alice!
Hello {{.APIKey}}, alice!
```

The second greeting prints the braces as text. The template was parsed once, from a constant, so the input cannot add an action.

- Verdict: stdlib. Go has no `eval`. Code injection in Go programs comes from template text, from plugins and from running other interpreters with input.
- The security model of `html/template` assumes that template authors are trusted and the data passed to `Execute` is not. `text/template` has the same model and does not escape the output for HTML.
- A template can read the exported fields and map keys of its data, and it can call methods. Pass a small view struct with only the values to print, not a whole domain object.
- Functions that a template can call come from the `FuncMap` that the program installs. Do not add functions that read files, run commands or reach other data.
- When users must write their own templates, give them a smaller language, such as a fixed set of `$name` placeholders, and replace them with `strings.NewReplacer`.
