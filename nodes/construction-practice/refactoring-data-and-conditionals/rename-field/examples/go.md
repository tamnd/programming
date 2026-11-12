```go run id=say-what-it-is
package main

import (
	"encoding/json"
	"fmt"
)

// The Go field is renamed from Name to Title. The struct tag keeps the
// stored JSON key "name" until the stored data is migrated.
type Organization struct {
	Title   string `json:"name"`
	Country string `json:"country"`
}

func main() {
	var org Organization
	if err := json.Unmarshal([]byte(`{"name":"Harbour Light Trust","country":"NO"}`), &org); err != nil {
		panic(err)
	}
	if org.Title != "Harbour Light Trust" {
		panic(org.Title)
	}
	out, _ := json.Marshal(org)
	if string(out) != `{"name":"Harbour Light Trust","country":"NO"}` {
		panic(string(out))
	}
	fmt.Println("rename field: tests pass")
}
```

```output
rename field: tests pass
```
