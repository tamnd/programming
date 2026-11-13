```go run id=three-models
package main

import (
	"encoding/xml"
	"fmt"
	"io"
	"strconv"
	"strings"
)

const text = `<orders>
  <order id="1"><total>9.50</total></order>
  <order id="2"><total>20.00</total></order>
  <order id="3"><total>4.25</total></order>
</orders>`

type Order struct {
	ID    string  `xml:"id,attr"`
	Total float64 `xml:"total"`
}

func main() {
	// Tree: Unmarshal the whole document into structs.
	var all struct {
		Orders []Order `xml:"order"`
	}
	if err := xml.Unmarshal([]byte(text), &all); err != nil {
		panic(err)
	}
	sum := 0.0
	for _, o := range all.Orders {
		sum += o.Total
	}
	fmt.Println("tree:", sum)

	// Pull: ask the decoder for each token.
	d := xml.NewDecoder(strings.NewReader(text))
	pulled := 0.0
	for {
		tok, err := d.Token()
		if err == io.EOF {
			break
		}
		if t, ok := tok.(xml.StartElement); ok && t.Name.Local == "total" {
			var s string
			if err := d.DecodeElement(&s, &t); err != nil {
				panic(err)
			}
			v, _ := strconv.ParseFloat(s, 64)
			pulled += v
		}
	}
	fmt.Println("pull:", pulled)

	// Mixed: pull to each order, then decode only that element.
	d = xml.NewDecoder(strings.NewReader(text))
	for {
		tok, err := d.Token()
		if err == io.EOF {
			break
		}
		if t, ok := tok.(xml.StartElement); ok && t.Name.Local == "order" {
			var o Order
			if err := d.DecodeElement(&o, &t); err != nil {
				panic(err)
			}
			fmt.Printf("order %s: %.2f\n", o.ID, o.Total)
		}
	}
}
```

```output for=three-models
tree: 33.75
pull: 33.75
order 1: 9.50
order 2: 20.00
order 3: 4.25
```

- Verdict: stdlib. `xml.Unmarshal` reads the whole document into structs, which is a tree in the form of the program types. `Decoder.Token` is a pull parser: each call returns the next token, a `StartElement`, an `EndElement`, `CharData`, a `Comment` or a `ProcInst`. `DecodeElement` reads the element at the current position into a value. The last loop mixes the two: it pulls to each `order` and decodes only that element, so the program never holds the whole file. Go has no push parser in the standard library.
