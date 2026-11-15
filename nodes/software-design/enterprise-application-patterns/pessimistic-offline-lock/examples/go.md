```go run id=lock-table-with-timeout
package main

import (
	"fmt"
	"slices"
	"strings"
	"sync"
)

type lockError struct{ resource, holder string }

func (e *lockError) Error() string { return e.resource + " is locked by " + e.holder }

type lock struct {
	owner string
	taken int
}

// LockManager keeps the locks in a table, so that they last across requests.
// In a real application, the table is in the database and the resource is its primary key.
type LockManager struct {
	mu    sync.Mutex
	table map[string]lock
}

const timeout = 30 // minutes: a lock that is older than this has been abandoned

func (m *LockManager) Acquire(resource, owner string, now int) error {
	m.mu.Lock()
	defer m.mu.Unlock()
	if l, ok := m.table[resource]; ok && l.taken <= now-timeout {
		delete(m.table, resource)
	}
	l, ok := m.table[resource]
	if !ok {
		m.table[resource] = lock{owner, now}
		return nil
	}
	if l.owner != owner {
		return &lockError{resource, l.owner}
	}
	return nil
}

func (m *LockManager) ReleaseAll(owner string) {
	m.mu.Lock()
	defer m.mu.Unlock()
	for r, l := range m.table {
		if l.owner == owner {
			delete(m.table, r)
		}
	}
}

func (m *LockManager) Held() string {
	m.mu.Lock()
	defer m.mu.Unlock()
	var out []string
	for r, l := range m.table {
		out = append(out, r+" by "+l.owner)
	}
	slices.Sort(out)
	return strings.Join(out, ", ")
}

var locks = &LockManager{table: map[string]lock{}}

func clock(minute int) string { return fmt.Sprintf("10:%02d", minute) }

func openForEdit(minute int, who, resource string) {
	if err := locks.Acquire(resource, who, minute); err != nil {
		fmt.Printf("%s %s opens %s for edit: refused: %v\n", clock(minute), who, resource, err)
		return
	}
	fmt.Printf("%s %s opens %s for edit: lock granted\n", clock(minute), who, resource)
}

func closeEdit(minute int, who string) {
	locks.ReleaseAll(who)
	fmt.Printf("%s %s saves and closes: locks released\n", clock(minute), who)
}

func main() {
	openForEdit(0, "ann", "customer 1")
	openForEdit(5, "bob", "customer 1")
	closeEdit(12, "ann")
	openForEdit(15, "bob", "customer 1")
	openForEdit(20, "bob", "order 9")
	// Bob closes the browser and never comes back.
	openForEdit(40, "carol", "customer 1")
	openForEdit(50, "carol", "customer 1")
	fmt.Println("locks held:", locks.Held())
}
```

```output
10:00 ann opens customer 1 for edit: lock granted
10:05 bob opens customer 1 for edit: refused: customer 1 is locked by ann
10:12 ann saves and closes: locks released
10:15 bob opens customer 1 for edit: lock granted
10:20 bob opens order 9 for edit: lock granted
10:40 carol opens customer 1 for edit: refused: customer 1 is locked by bob
10:50 carol opens customer 1 for edit: lock granted
locks held: customer 1 by carol, order 9 by bob
```

- Verdict: idiom. A `sync.Mutex` makes each method of the lock manager one step, because many goroutines can serve requests at the same time. `lockError` holds the resource and the holder for the message.
- `Held` sorts the list, because the order of a range over a map is not defined.
