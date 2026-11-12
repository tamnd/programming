```go run id=talk-to-the-worker
package main

import "fmt"

type Entry struct {
	Day   int
	Cents int64
}

type Ledger struct{ entries []Entry }

func (l *Ledger) Add(day int, cents int64) { l.entries = append(l.entries, Entry{day, cents}) }

func (l *Ledger) Balance() int64 {
	var s int64
	for _, e := range l.entries {
		s += e.Cents
	}
	return s
}

// After: the Account methods that only forwarded are deleted,
// and the ledger is an exported field that clients use directly.
type Account struct {
	Owner  string
	Ledger Ledger
}

func main() {
	a := Account{Owner: "Ana"}
	a.Ledger.Add(1, 500) // before: a.Add(1, 500)
	a.Ledger.Add(3, -120)
	a.Ledger.Add(9, 40)
	if got := a.Ledger.Balance(); got != 420 {
		panic(got)
	}
	fmt.Println("remove middle man: tests pass")
}
```

```output
remove middle man: tests pass
```
