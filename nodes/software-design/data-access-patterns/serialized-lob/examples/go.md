```go run id=graph-in-one-column
package main

import (
	"encoding/json"
	"fmt"
	"strings"
)

// Department is one node of the graph. The json tags give the field names in the LOB.
type Department struct {
	Name     string        `json:"name"`
	Children []*Department `json:"children"`
}

func dept(name string, children ...*Department) *Department {
	if children == nil {
		children = []*Department{} // An empty list, not null, in the LOB.
	}
	return &Department{name, children}
}

type row struct{ name, departments string }

// A stand-in for the database: the customer table, with the departments as one text column.
var customer = map[int]row{}

// save writes the whole graph of departments as one value in one column.
func save(id int, name string, departments []*Department) int {
	lob, err := json.Marshal(departments)
	if err != nil {
		panic(err)
	}
	customer[id] = row{name, string(lob)}
	return len(lob)
}

func load(id int) (string, []*Department) {
	r := customer[id]
	var departments []*Department
	if err := json.Unmarshal([]byte(r.departments), &departments); err != nil {
		panic(err)
	}
	return r.name, departments
}

func paths(departments []*Department, prefix string, out []string) []string {
	for _, d := range departments {
		out = append(out, prefix+d.Name)
		out = paths(d.Children, prefix+d.Name+"/", out)
	}
	return out
}

func ancestors(departments []*Department, target string, above []string) ([]string, bool) {
	for _, d := range departments {
		if d.Name == target {
			found := make([]string, len(above))
			for i, a := range above {
				found[len(above)-1-i] = a
			}
			return found, true
		}
		if found, ok := ancestors(d.Children, target, append(above[:len(above):len(above)], d.Name)); ok {
			return found, true
		}
	}
	return nil, false
}

func main() {
	tree := []*Department{dept("Sales", dept("Presales"), dept("Key accounts")), dept("Support")}
	size := save(1, "Acme", tree)
	fmt.Printf("saved Acme: %d bytes in one column\n", size)
	fmt.Println("column:", customer[1].departments)
	name, loaded := load(1)
	fmt.Printf("loaded %s: %s\n", name, strings.Join(paths(loaded, "", nil), " | "))
	up, _ := ancestors(loaded, "Presales", nil)
	fmt.Println("ancestors of Presales:", strings.Join(up, ", "))
	loaded[1].Children = append(loaded[1].Children, dept("Field"))
	size = save(1, name, loaded)
	fmt.Printf("add Support/Field: %d bytes written again\n", size)
	fmt.Println("rows in customer:", len(customer))
}
```

```output
saved Acme: 136 bytes in one column
column: [{"name":"Sales","children":[{"name":"Presales","children":[]},{"name":"Key accounts","children":[]}]},{"name":"Support","children":[]}]
loaded Acme: Sales | Sales/Presales | Sales/Key accounts | Support
ancestors of Presales: Sales
add Support/Field: 166 bytes written again
rows in customer: 1
```

- Verdict: idiom. `encoding/json` writes the tree from the struct tags, and `json.Unmarshal` makes it again. `dept` sets an empty slice for a leaf, so that the column has `[]` and not `null`.
- `ancestors` copies the path before each `append`, so that the calls for two siblings do not share one backing array.
