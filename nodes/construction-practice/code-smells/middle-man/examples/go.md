```go run id=remove-the-middle
package main

import (
	"fmt"
	"slices"
)

type Song struct {
	Title   string
	Seconds int
}

// Before: four methods forward to the slice, and one has a rule.
type PlaylistBefore struct{ songs []Song }

func (p *PlaylistBefore) Add(s Song)           { p.songs = append(p.songs, s) }
func (p *PlaylistBefore) Len() int             { return len(p.songs) }
func (p *PlaylistBefore) At(i int) Song        { return p.songs[i] }
func (p *PlaylistBefore) Contains(s Song) bool { return slices.Contains(p.songs, s) }
func (p *PlaylistBefore) TotalSeconds() int {
	t := 0
	for _, s := range p.songs {
		t += s.Seconds
	}
	return t
}

// After: a named slice type. Clients use len, indexing and the slices package
// directly, and the type keeps only the rule.
type Playlist []Song

func (p Playlist) TotalSeconds() int {
	t := 0
	for _, s := range p {
		t += s.Seconds
	}
	return t
}

// Embedding: a struct that gets all the methods of a field for free.
type Library struct {
	Playlist // embedded; Library has TotalSeconds with no forwarding code
	Owner    string
}

func main() {
	songs := []Song{{"Intro", 90}, {"Theme", 200}, {"Outro", 60}}
	before := &PlaylistBefore{}
	for _, s := range songs {
		before.Add(s)
	}
	after := Playlist(slices.Clone(songs))

	if before.Len() != len(after) || before.At(2) != after[2] {
		panic("len or index")
	}
	if before.Contains(songs[1]) != slices.Contains(after, songs[1]) {
		panic("contains")
	}
	if before.TotalSeconds() != after.TotalSeconds() || after.TotalSeconds() != 350 {
		panic("total")
	}
	lib := Library{Playlist: after, Owner: "ana"}
	if lib.TotalSeconds() != 350 {
		panic("promoted method")
	}
	fmt.Println("playlist: tests pass")
}
```

```output
playlist: tests pass
```

- `Playlist` is a defined type whose underlying type is `[]Song`, so `len`, indexing and the functions of the `slices` package work on it with no forwarding methods.
- `Library` embeds `Playlist`. The method `TotalSeconds` is promoted, so `lib.TotalSeconds()` works without a method on `Library`.
