```go run id=transfer-and-close
package main

import (
	"errors"
	"fmt"
	"maps"
	"slices"
	"strings"
)

type Account struct {
	ID      int
	Owner   string
	Balance int
}

type Entry struct {
	AccountID, Amount int
}

// A stand-in for the database. The Go standard library has database/sql,
// but no driver for a real database.
type accountRow struct {
	owner   string
	balance int
}

type Database struct {
	accounts map[int]accountRow
	entries  []Entry
}

// Transaction keeps a copy of the tables and puts it back when work fails.
func (db *Database) Transaction(work func() error) error {
	savedAccounts, savedEntries := maps.Clone(db.accounts), slices.Clone(db.entries)
	if err := work(); err != nil {
		db.accounts, db.entries = savedAccounts, savedEntries
		return err
	}
	return nil
}

func (db *Database) updateAccount(id int, owner string, balance int) error {
	if balance < 0 {
		return errors.New("CHECK constraint failed: balance >= 0")
	}
	db.accounts[id] = accountRow{owner, balance}
	return nil
}

func findAccount(db *Database, id int) *Account {
	row := db.accounts[id]
	return &Account{id, row.owner, row.balance}
}

// The mapper functions write each kind of object.
// A type switch chooses the code for the type of the object.
func insert(db *Database, obj any) error {
	switch o := obj.(type) {
	case *Entry:
		fmt.Printf("  insert entries (%d, %d)\n", o.AccountID, o.Amount)
		db.entries = append(db.entries, *o)
		return nil
	}
	return fmt.Errorf("no insert for %T", obj)
}

func update(db *Database, obj any) error {
	switch o := obj.(type) {
	case *Account:
		fmt.Printf("  update accounts %d %s %d\n", o.ID, o.Owner, o.Balance)
		return db.updateAccount(o.ID, o.Owner, o.Balance)
	}
	return fmt.Errorf("no update for %T", obj)
}

func remove(db *Database, obj any) error {
	switch o := obj.(type) {
	case *Account:
		fmt.Printf("  delete accounts %d\n", o.ID)
		delete(db.accounts, o.ID)
		return nil
	}
	return fmt.Errorf("no delete for %T", obj)
}

// UnitOfWork holds pointers, so == compares identity, not field values.
type UnitOfWork struct {
	db                   *Database
	created, dirty, gone []any
}

func (u *UnitOfWork) RegisterNew(obj any) { u.created = append(u.created, obj) }

func (u *UnitOfWork) RegisterDirty(obj any) {
	if !slices.Contains(u.created, obj) && !slices.Contains(u.dirty, obj) {
		u.dirty = append(u.dirty, obj)
	}
}

func (u *UnitOfWork) RegisterRemoved(obj any) {
	if slices.Contains(u.created, obj) { // never written, nothing to delete
		u.created = slices.DeleteFunc(u.created, func(o any) bool { return o == obj })
		return
	}
	u.dirty = slices.DeleteFunc(u.dirty, func(o any) bool { return o == obj }) // no update before the delete
	u.gone = append(u.gone, obj)
}

func (u *UnitOfWork) Commit() {
	err := u.db.Transaction(func() error {
		for _, step := range []struct {
			objs  []any
			write func(*Database, any) error
		}{{u.created, insert}, {u.dirty, update}, {u.gone, remove}} {
			for _, obj := range step.objs {
				if err := step.write(u.db, obj); err != nil {
					return err
				}
			}
		}
		return nil
	})
	u.created, u.dirty, u.gone = nil, nil, nil
	if err != nil {
		fmt.Println("  rollback:", err)
		return
	}
	fmt.Println("  commit")
}

func transfer(u *UnitOfWork, source, target *Account, amount int) {
	source.Balance -= amount
	target.Balance += amount
	u.RegisterDirty(source)
	u.RegisterDirty(target)
	u.RegisterNew(&Entry{source.ID, -amount})
	u.RegisterNew(&Entry{target.ID, amount})
}

func show(db *Database) {
	var accounts, entries []string
	for _, id := range slices.Sorted(maps.Keys(db.accounts)) {
		accounts = append(accounts, fmt.Sprintf("%d %s %d", id, db.accounts[id].owner, db.accounts[id].balance))
	}
	for _, e := range db.entries {
		entries = append(entries, fmt.Sprintf("%d %d", e.AccountID, e.Amount))
	}
	fmt.Println("accounts: " + strings.Join(accounts, " | "))
	fmt.Println("entries: " + strings.Join(entries, " | "))
}

func main() {
	db := &Database{accounts: map[int]accountRow{1: {"ann", 100}, 2: {"bob", 0}}}

	u := &UnitOfWork{db: db}
	transfer(u, findAccount(db, 1), findAccount(db, 2), 30)
	fmt.Printf("pending: %d new, %d dirty; entries in the table: %d\n", len(u.created), len(u.dirty), len(db.entries))
	fmt.Println("commit 1")
	u.Commit()

	u = &UnitOfWork{db: db}
	transfer(u, findAccount(db, 2), findAccount(db, 1), 50) // bob has only 30
	fmt.Println("commit 2")
	u.Commit()
	show(db)

	u = &UnitOfWork{db: db}
	ann, bob := findAccount(db, 1), findAccount(db, 2)
	transfer(u, bob, ann, 30) // close bob's account: move the money, then remove it
	u.RegisterRemoved(bob)
	fmt.Println("commit 3")
	u.Commit()
	show(db)
}
```

```output
pending: 2 new, 2 dirty; entries in the table: 0
commit 1
  insert entries (1, -30)
  insert entries (2, 30)
  update accounts 1 ann 70
  update accounts 2 bob 30
  commit
commit 2
  insert entries (2, -50)
  insert entries (1, 50)
  update accounts 2 bob -20
  rollback: CHECK constraint failed: balance >= 0
accounts: 1 ann 70 | 2 bob 30
entries: 1 -30 | 2 30
commit 3
  insert entries (2, -30)
  insert entries (1, 30)
  update accounts 1 ann 100
  delete accounts 2
  commit
accounts: 1 ann 100
entries: 1 -30 | 2 30 | 2 -30 | 1 30
```

- Verdict: idiom. The lists hold pointers in `any` values, so `slices.Contains` and `==` compare identity, not field values.
- The mapper functions use a type switch to choose the code for each kind of object, and `Commit` stops at the first error, which `Transaction` turns into a rollback.
