```go run id=one-struct-four-formats
package main

import (
	"bytes"
	"encoding/csv"
	"encoding/gob"
	"encoding/json"
	"encoding/xml"
	"fmt"
	"strconv"
	"strings"
)

type User struct {
	XMLName xml.Name `json:"-" xml:"user"`
	ID      int      `json:"id" xml:"id,attr"`
	Name    string   `json:"name" xml:"name"`
	Tags    []string `json:"tags" xml:"tag"`
}

func main() {
	u := User{ID: 42, Name: "Ada", Tags: []string{"admin", "ops"}}

	j, _ := json.Marshal(u)
	x, _ := xml.Marshal(u)

	var c bytes.Buffer
	w := csv.NewWriter(&c)
	w.Write([]string{"id", "name", "tags"})
	w.Write([]string{strconv.Itoa(u.ID), u.Name, strings.Join(u.Tags, ";")})
	w.Flush()

	// gob is binary and Go only. The first value on a stream also carries its type.
	var g bytes.Buffer
	enc := gob.NewEncoder(&g)
	enc.Encode(u)
	first := g.Len()
	enc.Encode(u)

	fmt.Println("json", len(j), string(j))
	fmt.Println("xml ", len(x), string(x))
	fmt.Println("csv ", c.Len(), strings.ReplaceAll(c.String(), "\n", `\n`))
	fmt.Println("gob ", first, "then", g.Len()-first)

	var back User
	gob.NewDecoder(&g).Decode(&back)
	fmt.Printf("%+v\n", back.Tags)

	// Without a Go type, a JSON number becomes float64.
	var any map[string]interface{}
	json.Unmarshal(j, &any)
	fmt.Printf("%T %T\n", any["id"], any["tags"])
}
```

```output for=one-struct-four-formats
json 45 {"id":42,"name":"Ada","tags":["admin","ops"]}
xml  67 <user id="42"><name>Ada</name><tag>admin</tag><tag>ops</tag></user>
csv  30 id,name,tags\n42,Ada,admin;ops\n
gob  144 then 25
[admin ops]
float64 []interface {}
```

- Verdict: stdlib. The packages `encoding/json`, `encoding/xml` and `encoding/csv` write the same struct as text, with struct tags that name the fields of each format. CSV has no list, so the program joins the tags itself. The Go-only `encoding/gob` is binary: the first value on a stream also carries a description of its type, so it takes 144 bytes, and the second value of the same type takes only 25. Without a Go type, `json.Unmarshal` gives a `float64` for a number and `[]interface {}` for an array.
