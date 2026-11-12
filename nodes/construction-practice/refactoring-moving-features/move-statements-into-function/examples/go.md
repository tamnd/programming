```go run id=always-together
package main

import (
	"fmt"
	"slices"
)

type Address struct{ Street, City, Postcode string }

type Recipient struct {
	Name    string
	Address Address
}

// After: the name line moved in, so the parameter is now the recipient.
func AddressBlock(r Recipient) []string {
	return []string{r.Name, r.Address.Street, r.Address.Postcode + " " + r.Address.City}
}

func main() {
	ana := Recipient{"Ana Pires", Address{"Rua Augusta 10", "Lisboa", "1100-053"}}
	want := []string{"Ana Pires", "Rua Augusta 10", "1100-053 Lisboa"}
	if got := AddressBlock(ana); !slices.Equal(got, want) {
		panic(fmt.Sprint(got))
	}
	invoice := append([]string{"Bill to:"}, AddressBlock(ana)...)
	if len(invoice) != 4 {
		panic(invoice)
	}
	fmt.Println("move statements into function: tests pass")
}
```

```output
move statements into function: tests pass
```
