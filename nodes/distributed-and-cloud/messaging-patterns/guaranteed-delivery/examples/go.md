```go run id=survive-crash
package main

import (
	"bufio"
	"fmt"
	"os"
	"path/filepath"
	"strings"
)

// durableQueue keeps its state in a log on disk: one line for each send and each ack.
type durableQueue struct {
	path    string
	order   []string
	pending map[string]string
}

func open(path string) (*durableQueue, error) {
	q := &durableQueue{path: path, pending: map[string]string{}}
	f, err := os.Open(path)
	if os.IsNotExist(err) {
		return q, nil
	} else if err != nil {
		return nil, err
	}
	defer f.Close()
	s := bufio.NewScanner(f)
	for s.Scan() {
		p := strings.SplitN(s.Text(), " ", 3)
		if p[0] == "send" {
			q.pending[p[1]] = p[2]
			q.order = append(q.order, p[1])
		} else {
			delete(q.pending, p[1])
		}
	}
	return q, s.Err()
}

func (q *durableQueue) log(line string) error {
	f, err := os.OpenFile(q.path, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0o600)
	if err != nil {
		return err
	}
	if _, err = f.WriteString(line + "\n"); err == nil {
		err = f.Sync()
	}
	if cerr := f.Close(); err == nil {
		err = cerr
	}
	return err
}

func (q *durableQueue) send(id, body string) error {
	if err := q.log("send " + id + " " + body); err != nil {
		return err
	}
	q.pending[id] = body
	q.order = append(q.order, id)
	return nil
}

func (q *durableQueue) ack(id string) error {
	if err := q.log("ack " + id); err != nil {
		return err
	}
	delete(q.pending, id)
	return nil
}

// ids gives the pending ids in the order they were sent.
func (q *durableQueue) ids() []string {
	var out []string
	for _, id := range q.order {
		if _, ok := q.pending[id]; ok {
			out = append(out, id)
		}
	}
	return out
}

func main() {
	dir, err := os.MkdirTemp("", "gd")
	if err != nil {
		panic(err)
	}
	defer os.RemoveAll(dir)
	path := filepath.Join(dir, "orders.log")

	memory := map[string]string{}
	durable, err := open(path)
	if err != nil {
		panic(err)
	}
	for i := 1; i <= 3; i++ {
		id, body := fmt.Sprintf("m%d", i), fmt.Sprintf("order-%d", i)
		memory[id] = body
		if err := durable.send(id, body); err != nil {
			panic(err)
		}
	}
	fmt.Printf("before crash: memory %d, durable %d\n", len(memory), len(durable.pending))

	memory = map[string]string{} // a crash loses what was only in memory
	if durable, err = open(path); err != nil {
		panic(err)
	}
	fmt.Printf("after crash: memory %d, durable %d\n", len(memory), len(durable.pending))

	id := durable.ids()[0]
	fmt.Printf("receive %s %s, ack\n", id, durable.pending[id])
	if err := durable.ack(id); err != nil {
		panic(err)
	}

	if durable, err = open(path); err != nil {
		panic(err)
	}
	fmt.Println("after second crash: durable", len(durable.pending), strings.Join(durable.ids(), " "))
}
```

```output
before crash: memory 3, durable 3
after crash: memory 0, durable 3
receive m1 order-1, ack
after second crash: durable 2 m2 m3
```

- Verdict: idiom. Go has no persistent queue. A log file with `File.Sync` after each line is the store, and `open` reads it again after a crash.
- `log` returns the error of `Sync` and of `Close`. A program that ignores them can tell the sender that a message is safe when the write failed.
- A Go map has no order, so the queue keeps a separate slice of the ids in the order they were sent.
