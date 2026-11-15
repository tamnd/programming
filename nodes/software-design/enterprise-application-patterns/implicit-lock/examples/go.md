```go run id=locking-mapper
package main

import (
	"errors"
	"fmt"
)

// LockError reports a lock that another business transaction holds.
type LockError struct {
	resource, holder string
}

func (e *LockError) Error() string {
	return e.resource + " is locked by " + e.holder
}

// lockManager keeps exclusive locks for business transactions until the transaction ends.
type lockManager struct {
	held map[string]string
}

func (m *lockManager) acquire(resource, owner string) error {
	holder, ok := m.held[resource]
	if !ok {
		m.held[resource] = owner
		return nil
	}
	if holder != owner {
		return &LockError{resource, holder}
	}
	return nil
}

func (m *lockManager) releaseAll(owner string) {
	for r, o := range m.held {
		if o == owner {
			delete(m.held, r)
		}
	}
}

type customer struct {
	id     int
	name   string
	credit int
}

type mapper interface {
	find(id int) (customer, error)
	update(c customer)
}

type customerMapper struct {
	rows map[int]customer
}

func (m *customerMapper) find(id int) (customer, error) { return m.rows[id], nil }
func (m *customerMapper) update(c customer)             { m.rows[c.id] = c }

var taken = 0

// lockingMapper wraps a mapper. Each find takes the lock first, so the application scripts never call the lock manager.
type lockingMapper struct {
	mapper mapper
	locks  *lockManager
	owner  string
}

func (m *lockingMapper) find(id int) (customer, error) {
	if err := m.locks.acquire(fmt.Sprintf("customer %d", id), m.owner); err != nil {
		return customer{}, err
	}
	taken++
	return m.mapper.find(id)
}

func (m *lockingMapper) update(c customer) { m.mapper.update(c) }

var (
	locks = &lockManager{held: map[string]string{}}
	plain = &customerMapper{rows: map[int]customer{1: {id: 1, name: "Acme Ltd", credit: 500}}}
)

// businessTransaction is what the framework gives to each business transaction:
// a locking mapper, and the release of its locks at the end.
type businessTransaction struct {
	owner     string
	customers mapper
}

func newBusinessTransaction(owner string) *businessTransaction {
	return &businessTransaction{owner, &lockingMapper{plain, locks, owner}}
}

func (tx *businessTransaction) end() { locks.releaseAll(tx.owner) }

// The application scripts. Not one of them calls the lock manager.
func openForRename(tx *businessTransaction, id int) (customer, error) {
	return tx.customers.find(id)
}

func saveRename(tx *businessTransaction, c customer, name string) {
	c.name = name
	tx.customers.update(c)
	tx.end()
}

func raiseCredit(tx *businessTransaction, id, amount int) (int, error) {
	c, err := tx.customers.find(id)
	if err != nil {
		return 0, err
	}
	c.credit += amount
	tx.customers.update(c)
	tx.end()
	return c.credit, nil
}

func runRaiseCredit(who string, tx *businessTransaction) {
	credit, err := raiseCredit(tx, 1, 100)
	var lockErr *LockError
	if errors.As(err, &lockErr) {
		tx.end()
		fmt.Printf("%s raises the credit of customer 1: refused: %v\n", who, err)
		return
	}
	fmt.Printf("%s raises the credit of customer 1: ok, credit %d\n", who, credit)
}

func main() {
	ann, bob := newBusinessTransaction("ann"), newBusinessTransaction("bob")
	c, err := openForRename(ann, 1)
	if err != nil {
		panic(err)
	}
	fmt.Println("ann opens customer 1 to rename it:", c.name)
	runRaiseCredit("bob", bob)
	saveRename(ann, c, "Acme Group")
	fmt.Println("ann saves the new name: Acme Group")
	runRaiseCredit("bob", bob)
	last, _ := plain.find(1)
	fmt.Println("final:", last.name, "with credit", last.credit)
	fmt.Println("locks taken by the mapper:", taken, "locks held now:", len(locks.held))
}
```

```output
ann opens customer 1 to rename it: Acme Ltd
bob raises the credit of customer 1: refused: customer 1 is locked by ann
ann saves the new name: Acme Group
bob raises the credit of customer 1: ok, credit 600
final: Acme Group with credit 600
locks taken by the mapper: 2 locks held now: 0
```

- Verdict: idiom. The `mapper` interface has `find` and `update`, and `lockingMapper` wraps another `mapper`. The scripts hold the interface, so they do not know which mapper they use.
- `runRaiseCredit` uses `errors.As` to find a `*LockError` in the error that it gets.
