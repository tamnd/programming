```go run id=processor
package main

import (
	"fmt"
	"strings"
)

type Command interface {
	Name() string
	Execute()
	Undo()
}

type Insert struct {
	words *[]string
	word  string
}

func (c Insert) Name() string { return "insert " + c.word }
func (c Insert) Execute()     { *c.words = append(*c.words, c.word) }
func (c Insert) Undo()        { *c.words = (*c.words)[:len(*c.words)-1] }

type Processor struct {
	done []Command
}

func (p *Processor) DoIt(c Command) {
	fmt.Println("do:", c.Name())
	c.Execute()
	p.done = append(p.done, c)
}

func (p *Processor) UndoIt() {
	c := p.done[len(p.done)-1]
	p.done = p.done[:len(p.done)-1]
	fmt.Println("undo:", c.Name())
	c.Undo()
}

func main() {
	var words []string
	p := &Processor{}
	p.DoIt(Insert{&words, "hello"})
	p.DoIt(Insert{&words, "world"})
	fmt.Println("text:", strings.Join(words, " "))
	p.UndoIt()
	fmt.Println("text:", strings.Join(words, " "))
}
```

```output
do: insert hello
do: insert world
text: hello world
undo: insert world
text: hello
```

- Verdict: idiom. `Command` is an interface, and the processor keeps a slice of executed commands as a stack.
