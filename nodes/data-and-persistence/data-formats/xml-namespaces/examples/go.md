```go run id=expanded-names
package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"strings"
)

const text = `<catalog xmlns="urn:example:books" xmlns:m="urn:example:meta">
  <book id="b1" m:lang="en">
    <title>Tide tables</title>
    <price xmlns="urn:example:money" currency="EUR">32.18</price>
  </book>
</catalog>`

type Book struct {
	XMLName xml.Name `xml:"urn:example:books book"`
	ID      string   `xml:"id,attr"`
	Lang    string   `xml:"urn:example:meta lang,attr"`
	Title   string   `xml:"urn:example:books title"`
	Price   string   `xml:"urn:example:money price,omitempty"`
}

type Catalog struct {
	XMLName xml.Name `xml:"urn:example:books catalog"`
	Books   []Book   `xml:"urn:example:books book"`
}

func main() {
	decoder := xml.NewDecoder(strings.NewReader(text))
	for {
		token, err := decoder.Token()
		if err == io.EOF {
			break
		}
		if start, ok := token.(xml.StartElement); ok {
			fmt.Printf("%s | %s\n", start.Name.Space, start.Name.Local)
		}
	}

	var catalog Catalog
	if err := xml.Unmarshal([]byte(text), &catalog); err != nil {
		fmt.Println("error:", err)
		return
	}
	book := catalog.Books[0]
	fmt.Println(book.ID, book.Lang, book.Title, book.Price)

	wrong := strings.Replace(text, "urn:example:books", "urn:example:other", 1)
	err := xml.Unmarshal([]byte(wrong), &catalog)
	fmt.Println("error:", err)

	out, _ := xml.Marshal(Book{ID: "b2", Lang: "fr", Title: "Moon phases"})
	fmt.Println(string(out))
}
```

```output for=expanded-names
urn:example:books | catalog
urn:example:books | book
urn:example:books | title
urn:example:money | price
b1 en Tide tables 32.18
error: expected element <catalog> in name space urn:example:books but have urn:example:other
<book xmlns="urn:example:books" id="b2" xmlns:_="urn:example:meta" _:lang="fr"><title xmlns="urn:example:books">Moon phases</title></book>
```

- Verdict: stdlib. `encoding/xml` gives each name as an `xml.Name` with the fields `Space` (the namespace name) and `Local`. A struct tag of the form `xml:"namespace local"` matches only the elements in that namespace, and `Unmarshal` returns an error when the root is in another namespace. `Marshal` writes the namespaces in its own way: it puts an `xmlns` attribute on each element and makes a prefix such as `_` for an attribute in a namespace. The output has the correct expanded names, but it is not the text that a person writes.
