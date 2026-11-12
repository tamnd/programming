```go run id=struct-tags-and-gob
package main

import (
	"bytes"
	"encoding/gob"
	"encoding/json"
	"fmt"
	"strings"
)

type Person struct {
	Name  string   `json:"name"`
	Born  int      `json:"born"`
	Died  int      `json:"died,omitempty"`
	Tags  []string `json:"tags"`
	notes string   // not exported, so not serialized
}

func main() {
	ada := Person{Name: "Ada", Born: 1815, Tags: []string{"math"}, notes: "private"}
	data, _ := json.Marshal(ada)
	fmt.Println(string(data))
	var back Person
	_ = json.Unmarshal(data, &back)
	fmt.Printf("%s %d %v notes=%q\n", back.Name, back.Born, back.Tags, back.notes)

	newer := `{"name":"Alan","born":1912,"field":"logic"}`
	var alan Person
	err := json.Unmarshal([]byte(newer), &alan)
	fmt.Println(alan.Name, alan.Born, "tags nil:", alan.Tags == nil, "err:", err)
	dec := json.NewDecoder(strings.NewReader(newer))
	dec.DisallowUnknownFields()
	fmt.Println("strict:", dec.Decode(&Person{}))

	var buf bytes.Buffer // gob: a binary format for Go programs
	_ = gob.NewEncoder(&buf).Encode(ada)
	var g Person
	_ = gob.NewDecoder(&buf).Decode(&g)
	fmt.Printf("gob: %s %d %v\n", g.Name, g.Born, g.Tags)
	err = gob.NewEncoder(&buf).Encode(func() {})
	fmt.Println("gob func:", err != nil)
}
```

```output for=struct-tags-and-gob
{"name":"Ada","born":1815,"tags":["math"]}
Ada 1815 [math] notes=""
Alan 1912 tags nil: true err: <nil>
strict: json: unknown field "field"
gob: Ada 1815 [math]
gob func: true
```

- Verdict: stdlib. `encoding/json` reads the exported fields of a struct through reflection, and struct tags give the JSON names and options such as `omitempty`. Fields that are not exported are skipped. Unknown fields are ignored, and missing ones keep their zero values, unless the decoder calls `DisallowUnknownFields`. `encoding/gob` is a binary format for Go programs that sends a description of each type with the data.
