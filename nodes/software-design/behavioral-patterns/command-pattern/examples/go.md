```go run id=editor
package main

import "fmt"

type Buffer struct{ Text string }

type Command interface {
	Execute()
	Undo()
}

type Append struct {
	buffer *Buffer
	text   string
}

func (a *Append) Execute() { a.buffer.Text += a.text }
func (a *Append) Undo()    { a.buffer.Text = a.buffer.Text[:len(a.buffer.Text)-len(a.text)] }

type Delete struct {
	buffer  *Buffer
	count   int
	removed string
}

func (d *Delete) Execute() {
	start := len(d.buffer.Text) - d.count
	d.removed = d.buffer.Text[start:]
	d.buffer.Text = d.buffer.Text[:start]
}

func (d *Delete) Undo() { d.buffer.Text += d.removed }

type History struct{ undo, redo []Command }

func (h *History) Run(c Command) {
	c.Execute()
	h.undo = append(h.undo, c)
	h.redo = h.redo[:0]
}

func (h *History) Undo() bool {
	if len(h.undo) == 0 {
		return false
	}
	c := h.undo[len(h.undo)-1]
	h.undo = h.undo[:len(h.undo)-1]
	c.Undo()
	h.redo = append(h.redo, c)
	return true
}

func (h *History) Redo() bool {
	if len(h.redo) == 0 {
		return false
	}
	c := h.redo[len(h.redo)-1]
	h.redo = h.redo[:len(h.redo)-1]
	c.Execute()
	h.undo = append(h.undo, c)
	return true
}

func main() {
	buffer := &Buffer{}
	history := &History{}
	show := func(label string) { fmt.Printf("%s: %q\n", label, buffer.Text) }
	history.Run(&Append{buffer, "hello"})
	show("append")
	history.Run(&Append{buffer, " world"})
	show("append")
	history.Run(&Delete{buffer: buffer, count: 5})
	show("delete")
	history.Undo()
	show("undo")
	history.Undo()
	show("undo")
	history.Redo()
	show("redo")
	history.Run(&Append{buffer, "!"})
	show("append")
	if !history.Redo() {
		fmt.Println("redo: nothing to redo")
	}
}
```

```output for=editor
append: "hello"
append: "hello world"
delete: "hello "
undo: "hello world"
undo: "hello"
redo: "hello world"
append: "hello world!"
redo: nothing to redo
```

- Verdict: idiom. An interface with two methods, and a struct for each kind of command. The methods have pointer receivers, so `Delete` can save the removed text in itself.
- The two stacks are slices. `h.redo[:0]` empties the redo stack and keeps its array for reuse.
- `%q` quotes the text with Go's escape rules. For this text the result is the same as the quotes in the other examples.
- A command with no undo is a `func()`, and a slice of `func()` values is a queue of commands. Undo needs a second function, so this file uses an interface.
