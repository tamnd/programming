```go run id=parse-emit
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

type Order struct {
	ID    int64    `json:"id"`
	Total float64  `json:"total"`
	Tags  []string `json:"tags"`
	Note  *string  `json:"note,omitempty"`
}

func main() {
	text := `{"id": 9007199254740993, "total": 0.10, "tags": ["new"], "note": null}`

	var order Order
	if err := json.Unmarshal([]byte(text), &order); err != nil {
		fmt.Println(err)
		return
	}
	fmt.Printf("%+v\n", order)

	out, _ := json.Marshal(order)
	fmt.Println(string(out))

	var loose map[string]any
	json.Unmarshal([]byte(text), &loose)
	fmt.Println(loose["id"])

	decoder := json.NewDecoder(strings.NewReader(text))
	decoder.UseNumber()
	var exact map[string]any
	decoder.Decode(&exact)
	fmt.Println(exact["id"])
}
```

```output for=parse-emit
{ID:9007199254740993 Total:0.1 Tags:[new] Note:<nil>}
{"id":9007199254740993,"total":0.1,"tags":["new"]}
9.007199254740992e+15
9007199254740993
```

```go run id=round-trip
package main

import (
	"encoding/json"
	"fmt"
	"math"
	"strings"
)

type Order struct {
	ID int64 `json:"id"`
}

func main() {
	var order Order
	err := json.Unmarshal([]byte(`{"ID": 1, "id": 2, "Id": 3, "extra": true}`), &order)
	fmt.Println(order.ID, err)

	strict := json.NewDecoder(strings.NewReader(`{"id": 1, "extra": true}`))
	strict.DisallowUnknownFields()
	fmt.Println(strict.Decode(&order))

	out, _ := json.Marshal(map[string]string{"html": "<b>&</b>"})
	fmt.Println(string(out))

	_, err = json.Marshal(math.NaN())
	fmt.Println(err)

	var text string
	json.Unmarshal([]byte("\"\xff\""), &text)
	fmt.Printf("%q\n", text)
}
```

```output for=round-trip
3 <nil>
json: unknown field "extra"
{"html":"\u003cb\u003e\u0026\u003c/b\u003e"}
json: unsupported value: NaN
"�"
```

- Verdict: stdlib. The package `encoding/json` has `Marshal` and `Unmarshal` for byte slices, and `Encoder` and `Decoder` for streams. Struct tags name the JSON members.
- A typed field keeps the integer exactly. A value decoded into `any` becomes a `float64`, so `9007199254740993` loses its last digit. `Decoder.UseNumber` keeps the text as a `json.Number`.
- `Unmarshal` matches member names to fields without regard to case, and a later member overwrites an earlier one. In the second example `"ID"`, `"id"` and `"Id"` all fill the field `ID`, and the last one wins.
- Unknown members are ignored unless the decoder calls `DisallowUnknownFields`.
- `Marshal` escapes `<`, `>` and `&` so the output is safe inside HTML. It returns an error for `NaN` and the infinities.
- Invalid UTF-8 in a string becomes U+FFFD with no error.
- The package `encoding/json/v2` has stricter defaults: it matches names with case, and it rejects duplicate names and invalid UTF-8.
