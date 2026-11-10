```go wrong id=sprintf
package main

import (
	"database/sql"
	"database/sql/driver"
	"fmt"
	"io"
)

// recorder is a fake driver. It prints what database/sql sends to a
// driver, so the example needs no database server and no cgo.
type recorder struct{}

func (recorder) Open(string) (driver.Conn, error) { return conn{}, nil }

type conn struct{}

func (conn) Prepare(query string) (driver.Stmt, error) { return stmt{query}, nil }
func (conn) Close() error                              { return nil }
func (conn) Begin() (driver.Tx, error)                 { return nil, driver.ErrSkip }

type stmt struct{ query string }

func (s stmt) Close() error  { return nil }
func (s stmt) NumInput() int { return -1 }
func (s stmt) Exec([]driver.Value) (driver.Result, error) {
	return nil, driver.ErrSkip
}
func (s stmt) Query(args []driver.Value) (driver.Rows, error) {
	fmt.Printf("query: %s\nargs:  %q\n", s.query, args)
	return rows{}, nil
}

type rows struct{}

func (rows) Columns() []string         { return []string{"id"} }
func (rows) Close() error              { return nil }
func (rows) Next([]driver.Value) error { return io.EOF }

func main() {
	sql.Register("recorder", recorder{})
	db, _ := sql.Open("recorder", "")
	customer := "x' OR '1'='1"

	query := fmt.Sprintf("SELECT id FROM orders WHERE customer = '%s'", customer)
	rows, _ := db.Query(query)
	rows.Close()
}
```

```output for=sprintf
query: SELECT id FROM orders WHERE customer = 'x' OR '1'='1'
args:  []
```

```go run id=placeholder
package main

import (
	"database/sql"
	"database/sql/driver"
	"fmt"
	"io"
)

// recorder is a fake driver. It prints what database/sql sends to a
// driver, so the example needs no database server and no cgo.
type recorder struct{}

func (recorder) Open(string) (driver.Conn, error) { return conn{}, nil }

type conn struct{}

func (conn) Prepare(query string) (driver.Stmt, error) { return stmt{query}, nil }
func (conn) Close() error                              { return nil }
func (conn) Begin() (driver.Tx, error)                 { return nil, driver.ErrSkip }

type stmt struct{ query string }

func (s stmt) Close() error  { return nil }
func (s stmt) NumInput() int { return -1 }
func (s stmt) Exec([]driver.Value) (driver.Result, error) {
	return nil, driver.ErrSkip
}
func (s stmt) Query(args []driver.Value) (driver.Rows, error) {
	fmt.Printf("query: %s\nargs:  %q\n", s.query, args)
	return rows{}, nil
}

type rows struct{}

func (rows) Columns() []string         { return []string{"id"} }
func (rows) Close() error              { return nil }
func (rows) Next([]driver.Value) error { return io.EOF }

func main() {
	sql.Register("recorder", recorder{})
	db, _ := sql.Open("recorder", "")
	customer := "x' OR '1'='1"

	rows, _ := db.Query("SELECT id FROM orders WHERE customer = ?", customer)
	rows.Close()
}
```

```output for=placeholder
query: SELECT id FROM orders WHERE customer = ?
args:  ["x' OR '1'='1"]
```

- Verdict: stdlib. The package `database/sql` defines the API, and a driver package connects it to a database. `Query`, `QueryRow` and `Exec` take the SQL text and then the arguments.
- The Go distribution has no database driver, so the examples register a fake driver that prints what it receives. With `Sprintf`, the driver gets one string with the input inside it and no arguments. With a placeholder, it gets the SQL text and the value apart.
- A real driver sends the value apart from the text, or binds it in the database library. The placeholder syntax belongs to the driver: `?` for MySQL and SQLite drivers, `$1` for PostgreSQL drivers.
- `db.Prepare` makes a statement once for many calls. `db.Query` with arguments also prepares the statement, unless the driver can send the text and the arguments in one step.
- The linter gosec reports SQL text built with `Sprintf` or `+` (rules G201 and G202).
