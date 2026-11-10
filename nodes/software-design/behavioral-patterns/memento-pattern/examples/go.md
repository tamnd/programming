```go run id=undo
package main

import "fmt"

// Memento holds a saved state. Its fields are unexported, so code in other
// packages can store a Memento but cannot read or change it.
type Memento struct {
	text   string
	cursor int
}

// Editor is the originator.
type Editor struct {
	text   string
	cursor int
}

func (e *Editor) Type(chars string) {
	e.text = e.text[:e.cursor] + chars + e.text[e.cursor:]
	e.cursor += len(chars)
}

func (e *Editor) MoveTo(position int) { e.cursor = max(0, min(position, len(e.text))) }

func (e *Editor) Save() Memento { return Memento{text: e.text, cursor: e.cursor} }

func (e *Editor) Restore(m Memento) { e.text, e.cursor = m.text, m.cursor }

func (e *Editor) Show() string { return e.text[:e.cursor] + "|" + e.text[e.cursor:] }

// History is the caretaker.
type History struct {
	editor *Editor
	undo   []Memento
}

func (h *History) Save() { h.undo = append(h.undo, h.editor.Save()) }

func (h *History) Undo() bool {
	if len(h.undo) == 0 {
		return false
	}
	last := h.undo[len(h.undo)-1]
	h.undo = h.undo[:len(h.undo)-1]
	h.editor.Restore(last)
	return true
}

func main() {
	editor := &Editor{}
	history := &History{editor: editor}

	history.Save()
	editor.Type("Hello")
	fmt.Println(editor.Show())
	history.Save()
	editor.Type(" world")
	fmt.Println(editor.Show())
	history.Save()
	editor.MoveTo(5)
	editor.Type(",")
	fmt.Println(editor.Show())

	for range 4 {
		if history.Undo() {
			fmt.Println("undo:", editor.Show())
		} else {
			fmt.Println("nothing to undo")
		}
	}
}
```

```output for=undo
Hello|
Hello world|
Hello,| world
undo: Hello world|
undo: Hello|
undo: |
nothing to undo
```

- Verdict: idiom. A struct with unexported fields is the narrow interface for code in other packages. In this one-file program everything is in package `main`, so the fields are visible to `History` too.
- The memento is a value, not a pointer. `Save` returns a copy of the fields, and Go strings are immutable, so the memento needs no deep copy.
- A state that holds a slice or a map needs `slices.Clone` or `maps.Clone` in `Save`. A struct copy shares the backing array of a slice.
- `Type` and `Show` index the string by bytes. That is correct here because the text is ASCII.
