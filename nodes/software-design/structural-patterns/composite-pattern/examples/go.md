```go run id=tree
package main

import (
	"fmt"
	"strings"
)

// Entry is the component.
type Entry interface {
	Size() int
	Lines(depth int) []string
}

type File struct {
	Name  string
	Bytes int
}

type Directory struct {
	Name     string
	Children []Entry
}

func (f File) Size() int { return f.Bytes }

func (f File) Lines(depth int) []string {
	return []string{fmt.Sprintf("%s%s %d", strings.Repeat("  ", depth), f.Name, f.Bytes)}
}

func (d *Directory) Size() int {
	total := 0
	for _, child := range d.Children {
		total += child.Size()
	}
	return total
}

func (d *Directory) Lines(depth int) []string {
	lines := []string{fmt.Sprintf("%s%s/ %d", strings.Repeat("  ", depth), d.Name, d.Size())}
	for _, child := range d.Children {
		lines = append(lines, child.Lines(depth+1)...)
	}
	return lines
}

func main() {
	project := &Directory{"project", []Entry{
		File{"readme.md", 200},
		&Directory{"src", []Entry{File{"main.go", 1200}, File{"util.go", 300}}},
	}}
	fmt.Println(strings.Join(project.Lines(0), "\n"))
}
```

```output for=tree
project/ 1700
  readme.md 200
  src/ 1500
    main.go 1200
    util.go 300
```

- Verdict: idiom. An interface is the component, and both types satisfy it. A directory holds a slice of the interface type, so it can hold files and directories.
- `Directory` has pointer receivers, so a program can add children to a directory that is already in the tree. A `File` is a value, because it has no children to change.
- The standard library has the same shape in `io/fs`: a directory and a file are both opened as an `fs.File`, and `fs.WalkDir` walks the tree.
