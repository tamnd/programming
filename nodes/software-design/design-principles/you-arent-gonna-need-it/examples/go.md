```go run id=receipt-email
package main

import "fmt"

type Order struct {
	Email string
	Total string
}

var sent []string

func sendReceipt(o Order) {
	sent = append(sent, fmt.Sprintf("email to %s: %s", o.Email, o.Total))
}

func main() {
	sendReceipt(Order{Email: "ana@example.org", Total: "12.50"})
	fmt.Println(sent[0])
}
```

```output
email to ana@example.org: 12.50
```
