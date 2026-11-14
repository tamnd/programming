```go run id=value-in-owner-columns
package main

import (
	"fmt"
	"sort"
	"strings"
)

// Value objects: structs without a key, compared with == by their fields.
type DateRange struct {
	Start string
	End   *string // nil: the employment has not ended.
}

func (d DateRange) String() string {
	end := "open"
	if d.End != nil {
		end = *d.End
	}
	return d.Start + " to " + end
}

type Money struct {
	Amount   int
	Currency string
}

func (m Money) String() string { return fmt.Sprintf("%d %s", m.Amount, m.Currency) }

// The row: the columns of the employment table.
type row struct {
	person, startDate string
	endDate           *string
	salaryAmount      int
	salaryCurrency    string
}

// A stand-in for the database: one table keyed by id.
var employmentTable = map[int64]row{}
var lastID int64

type Employment struct {
	ID     int64
	Person string
	Period DateRange
	Salary Money
}

func toRow(e *Employment) row {
	return row{e.Person, e.Period.Start, e.Period.End, e.Salary.Amount, e.Salary.Currency}
}

func insert(e *Employment) {
	lastID++
	employmentTable[lastID] = toRow(e)
	e.ID = lastID
}

func update(e *Employment) { employmentTable[e.ID] = toRow(e) }

func find(id int64) *Employment {
	r := employmentTable[id]
	return &Employment{id, r.person, DateRange{r.startDate, r.endDate}, Money{r.salaryAmount, r.salaryCurrency}}
}

func ids() []int64 {
	keys := make([]int64, 0, len(employmentTable))
	for k := range employmentTable {
		keys = append(keys, k)
	}
	sort.Slice(keys, func(i, j int) bool { return keys[i] < keys[j] })
	return keys
}

func main() {
	end := "2024-12-31"
	insert(&Employment{Person: "Ann", Period: DateRange{"2024-01-01", &end}, Salary: Money{5000, "EUR"}})
	insert(&Employment{Person: "Bo", Period: DateRange{"2025-03-01", nil}, Salary: Money{4000, "EUR"}})
	for _, id := range ids() {
		r := employmentTable[id]
		endDate := "null"
		if r.endDate != nil {
			endDate = *r.endDate
		}
		fmt.Printf("row: %d %s %s %s %d %s\n", id, r.person, r.startDate, endDate, r.salaryAmount, r.salaryCurrency)
	}
	ann, bo := find(1), find(2)
	for _, e := range []*Employment{ann, bo} {
		fmt.Printf("loaded: %s | %s | %s\n", e.Person, e.Period, e.Salary)
	}
	same := "no"
	if ann.Salary == (Money{5000, "EUR"}) {
		same = "yes"
	}
	fmt.Println("salary of Ann equals 5000 EUR:", same)

	ann.Salary = Money{5500, "EUR"} // A new value replaces the old one.
	update(ann)
	fmt.Printf("after raise: Ann %s | Bo %s\n", find(1).Salary, find(2).Salary)

	var above []string
	for _, id := range ids() {
		if r := employmentTable[id]; r.salaryCurrency == "EUR" && r.salaryAmount > 4500 {
			above = append(above, r.person)
		}
	}
	fmt.Println("EUR salaries above 4500:", strings.Join(above, ", "))
}
```

```output
row: 1 Ann 2024-01-01 2024-12-31 5000 EUR
row: 2 Bo 2025-03-01 null 4000 EUR
loaded: Ann | 2024-01-01 to 2024-12-31 | 5000 EUR
loaded: Bo | 2025-03-01 to open | 4000 EUR
salary of Ann equals 5000 EUR: yes
after raise: Ann 5500 EUR | Bo 4000 EUR
EUR salaries above 4500: Ann
```

- Verdict: idiom. The values are structs that the program passes by value, and `==` compares `Money` values field by field. The end date is a `*string`, so `nil` stands for a null column; `==` on a `DateRange` would compare this pointer and not the text.
- `toRow` copies the fields of the values into a flat `row` struct, and `find` builds new values from it.
