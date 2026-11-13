```go run id=parse-and-write
package main

import (
	"encoding/xml"
	"fmt"
)

type Book struct {
	XMLName xml.Name `xml:"book"`
	ID      string   `xml:"id,attr"`
	Title   string   `xml:"title"`
	Note    string   `xml:"note,omitempty"`
}

type Library struct {
	XMLName xml.Name `xml:"library"`
	City    string   `xml:"city,attr"`
	Books   []Book   `xml:"book"`
}

func main() {
	text := `<library city="Zürich">
  <book id="b1"><title>Tom &amp; Jerry</title><note><![CDATA[a < b]]></note></book>
  <book id="b2"><title>Go &#x2764;</title></book>
</library>`
	var lib Library
	if err := xml.Unmarshal([]byte(text), &lib); err != nil {
		fmt.Println("error:", err)
		return
	}
	fmt.Println(lib.City, len(lib.Books))
	for _, b := range lib.Books {
		fmt.Printf("%s %q %q\n", b.ID, b.Title, b.Note)
	}

	out, _ := xml.MarshalIndent(Book{ID: "b3", Title: "Q&A <draft>"}, "", "  ")
	fmt.Println(string(out))

	var bad Library
	fmt.Println("error:", xml.Unmarshal([]byte(`<library><book></library>`), &bad))
}
```

```output for=parse-and-write
Zürich 2
b1 "Tom & Jerry" "a < b"
b2 "Go ❤" ""
<book id="b3">
  <title>Q&amp;A &lt;draft&gt;</title>
</book>
error: XML syntax error on line 1: element <book> closed by </library>
```

- Verdict: stdlib. `encoding/xml` maps elements and attributes to struct fields by the field tags: `xml:"id,attr"` reads an attribute, and `xml:"book"` collects each `book` child into the slice. `Unmarshal` resolves the entity references, the character references and the CDATA section. `MarshalIndent` escapes `&`, `<` and `>` in text. Without the `XMLName` field, `Marshal` names the element after the Go type, here `Book`. A wrong end tag gives a syntax error with the line number.
