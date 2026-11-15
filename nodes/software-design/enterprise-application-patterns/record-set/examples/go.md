```go run id=account-record-set
package main

import (
	"fmt"
	"strings"
)

// RecordSet keeps rows and named columns in memory, in the shape of the result of a query.
type RecordSet struct {
	Columns []string
	Rows    [][]any
	States  []string
}

func NewRecordSet(columns []string, rows [][]any) *RecordSet {
	rs := &RecordSet{Columns: columns}
	for _, r := range rows {
		rs.Rows = append(rs.Rows, append([]any(nil), r...))
		rs.States = append(rs.States, "unchanged")
	}
	return rs
}

func (rs *RecordSet) index(column string) int {
	for i, c := range rs.Columns {
		if c == column {
			return i
		}
	}
	panic("no column " + column)
}

func (rs *RecordSet) Get(i int, column string) any { return rs.Rows[i][rs.index(column)] }

func (rs *RecordSet) Set(i int, column string, v any) {
	rs.Rows[i][rs.index(column)] = v
	if rs.States[i] == "unchanged" {
		rs.States[i] = "modified"
	}
}

func (rs *RecordSet) Add(values ...any) {
	rs.Rows = append(rs.Rows, values)
	rs.States = append(rs.States, "added")
}

func money(c int64) string { return fmt.Sprintf("%d.%02d", c/100, c%100) }

func main() {
	// The columns and rows as database/sql gives them for:
	// select id, name, balance from account order by id
	rs := NewRecordSet([]string{"id", "name", "balance"}, [][]any{
		{1, "Ann", int64(12000)},
		{2, "Bob", int64(1550)},
	})

	fmt.Println("columns:", strings.Join(rs.Columns, ", "))
	rs.Set(1, "balance", rs.Get(1, "balance").(int64)+1000)
	rs.Add(3, "Cy", int64(0))
	var rich []string
	saves := 0
	for i := range rs.Rows {
		fmt.Printf("%v | %v | %s | %s\n", rs.Get(i, "id"), rs.Get(i, "name"), money(rs.Get(i, "balance").(int64)), rs.States[i])
		if rs.Get(i, "balance").(int64) > 2000 {
			rich = append(rich, rs.Get(i, "name").(string))
		}
		if rs.States[i] != "unchanged" {
			saves++
		}
	}
	fmt.Println("balance over 20.00:", strings.Join(rich, ", "))
	fmt.Println("rows to save:", saves)
}
```

```output
columns: id, name, balance
1 | Ann | 120.00 | unchanged
2 | Bob | 25.50 | modified
3 | Cy | 0.00 | added
balance over 20.00: Ann, Bob
rows to save: 2
```

- Verdict: idiom. `database/sql` gives the names of the columns with `Rows.Columns` and the cells with `Rows.Scan`, but it does not keep the rows in memory. The `RecordSet` struct does this, with `any` cells.
- A type assertion such as `.(int64)` reads a cell as its type, and panics if the type is wrong.
