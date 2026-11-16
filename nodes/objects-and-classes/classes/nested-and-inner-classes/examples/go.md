```go run id=local-types-and-closures
package main

import "fmt"

type Playlist struct {
	name  string
	songs []string
}

// cursor is a separate type of the package. Its name and its field link it to Playlist.
type cursor struct {
	owner *Playlist
	i     int
}

func (c *cursor) Next() (string, bool) {
	if c.i >= len(c.owner.songs) {
		return "", false
	}
	c.i++
	return c.owner.name + ": " + c.owner.songs[c.i-1], true
}

func (p *Playlist) Cursor() *cursor { return &cursor{owner: p} }

func main() {
	p := &Playlist{name: "road", songs: []string{"one", "two"}}
	c := p.Cursor()
	for s, ok := c.Next(); ok; s, ok = c.Next() {
		fmt.Println(s)
	}

	// A type declared in a function is local to it. It cannot have methods,
	// so a closure does the job of a local or anonymous class.
	type pair struct{ a, b string }
	x := pair{"one", "two"}
	n := 0
	next := func() int { // an anonymous function that keeps n between calls
		n++
		return n
	}
	fmt.Println(x, next(), next())
}
```

```output for=local-types-and-closures
road: one
road: two
{one two} 1 2
```

- Verdict: idiom. Go has no nested types and no inner classes. A second type in the same package, with a lower-case name, does the job of a private nested class; code in the package can still use it.
- A type declared in a function cannot have methods, so Go code uses a closure where Java uses an anonymous class.
