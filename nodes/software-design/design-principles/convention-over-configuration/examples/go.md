```go run id=table-names
package main

import (
	"fmt"
	"reflect"
	"strings"
)

type Sale struct{}
type Order struct{}
type ProductSale struct{}

// TableName is the one deviation; it needs configuration.
func (ProductSale) TableName() string { return "product_sales" }

type tabler interface{ TableName() string }

// tableFor applies the convention: the type name in lower case, plus "s".
func tableFor(model any) string {
	if t, ok := model.(tabler); ok {
		return t.TableName()
	}
	return strings.ToLower(reflect.TypeOf(model).Name()) + "s"
}

func main() {
	for _, model := range []any{Sale{}, Order{}, ProductSale{}} {
		fmt.Printf("%s -> %s\n", reflect.TypeOf(model).Name(), tableFor(model))
	}
}
```

```output
Sale -> sales
Order -> orders
ProductSale -> product_sales
```
