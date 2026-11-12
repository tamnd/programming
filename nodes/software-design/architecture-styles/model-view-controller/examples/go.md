```go run id=todo
package main

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
)

type Item struct {
	Title string
	Done  bool
}

// TodoModel holds the items and calls its observers after each change.
type TodoModel struct {
	items     []Item
	observers []func()
}

func (m *TodoModel) Subscribe(f func()) { m.observers = append(m.observers, f) }

func (m *TodoModel) Items() []Item { return append([]Item(nil), m.items...) }

func (m *TodoModel) Add(title string) error {
	if title == "" {
		return errors.New("a title is required")
	}
	m.items = append(m.items, Item{Title: title})
	m.changed()
	return nil
}

func (m *TodoModel) Complete(n int) error {
	if n < 1 || n > len(m.items) {
		return fmt.Errorf("no item %d", n)
	}
	m.items[n-1].Done = true
	m.changed()
	return nil
}

func (m *TodoModel) changed() {
	for _, f := range m.observers {
		f()
	}
}

func listView(m *TodoModel) {
	m.Subscribe(func() {
		for i, item := range m.Items() {
			mark := " "
			if item.Done {
				mark = "x"
			}
			fmt.Printf("  [%s] %d %s\n", mark, i+1, item.Title)
		}
	})
}

func summaryView(m *TodoModel) {
	m.Subscribe(func() {
		items := m.Items()
		done := 0
		for _, item := range items {
			if item.Done {
				done++
			}
		}
		fmt.Printf("  summary: %d open, %d done\n", len(items)-done, done)
	})
}

type Controller struct{ model *TodoModel }

func (c Controller) Handle(line string) {
	fmt.Println(">", line)
	command, argument, _ := strings.Cut(line, " ")
	var err error
	switch n, convErr := strconv.Atoi(argument); {
	case command == "add":
		err = c.model.Add(argument)
	case command == "done" && convErr == nil:
		err = c.model.Complete(n)
	default:
		fmt.Printf("  error: cannot read '%s'\n", line)
	}
	if err != nil {
		fmt.Println("  error:", err)
	}
}

func main() {
	model := &TodoModel{}
	listView(model)
	summaryView(model)
	controller := Controller{model}
	for _, line := range []string{"add buy milk", "add write report", "done 1", "done 7", "rename 2"} {
		controller.Handle(line)
	}
}
```

```output
> add buy milk
  [ ] 1 buy milk
  summary: 1 open, 0 done
> add write report
  [ ] 1 buy milk
  [ ] 2 write report
  summary: 2 open, 0 done
> done 1
  [x] 1 buy milk
  [ ] 2 write report
  summary: 1 open, 1 done
> done 7
  error: no item 7
> rename 2
  error: cannot read 'rename 2'
```

- Verdict: idiom. The observers are a slice of functions. Each view is a function that subscribes a closure over the model.
- The model methods return an `error`, and the controller turns it into a message. The model has no idea where its errors are shown.
- `Items` returns a copy of the slice, so a view cannot change the stored items through the slice it gets.
- In a Go web server, an `http.Handler` plays the controller and an `html/template` plays the view. The node page shows that form.
