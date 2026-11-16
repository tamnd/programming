```go run id=stringer-and-verbs
package main

import "fmt"

type Book struct {
	Title string
	Year  int
}

// String is the display form; fmt calls it for %v and %s.
func (b Book) String() string {
	return fmt.Sprintf("%s (%d)", b.Title, b.Year)
}

type Plain struct {
	Title string
	Year  int
}

func main() {
	b := Book{"Dune", 1965}
	fmt.Println(b)
	fmt.Printf("%v | %s | %#v\n", b, b, b)
	p := Plain{"Dune", 1965}
	fmt.Printf("%v | %+v | %#v\n", p, p, p)
}
```

```output for=stringer-and-verbs
Dune (1965)
Dune (1965) | Dune (1965) | main.Book{Title:"Dune", Year:1965}
{Dune 1965} | {Title:Dune Year:1965} | main.Plain{Title:"Dune", Year:1965}
```

- Verdict: stdlib. A type with a `String() string` method implements `fmt.Stringer`, and `fmt` calls it for `%v` and `%s` and in `Println`.
- Without the method, `%v` prints the fields in braces, `%+v` adds the field names, and `%#v` prints the value as Go syntax. `%#v` ignores `String`; a `GoString` method can change it.
- The display form and the debug form are two verbs of one package, not two methods that the type must write.
