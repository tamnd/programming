```go run id=glyphs
package main

import (
	"fmt"
	"strings"
)

var widths = map[rune]int{'i': 1, 'l': 1, 'r': 1, 'm': 3, 'w': 3} // all other letters are 2 units wide

// Glyph is a flyweight. It holds only the intrinsic state: the character and its width.
type Glyph struct {
	char  rune
	width int
}

// Draw takes the column as extrinsic state.
func (g *Glyph) Draw(column int) string { return fmt.Sprintf("%c@%d", g.char, column) }

// GlyphFactory hands out one *Glyph for each character.
type GlyphFactory struct{ glyphs map[rune]*Glyph }

func (f *GlyphFactory) Get(char rune) *Glyph {
	if g, ok := f.glyphs[char]; ok {
		return g
	}
	width, ok := widths[char]
	if !ok {
		width = 2
	}
	g := &Glyph{char: char, width: width}
	f.glyphs[char] = g
	return g
}

func main() {
	factory := &GlyphFactory{glyphs: map[rune]*Glyph{}}
	var line []*Glyph
	for _, char := range "abracadabra" {
		line = append(line, factory.Get(char))
	}

	column := 0
	var drawn []string
	for _, g := range line {
		drawn = append(drawn, g.Draw(column))
		column += g.width
	}
	fmt.Println(strings.Join(drawn, " "))
	fmt.Printf("characters: %d, glyph objects: %d\n", len(line), len(factory.glyphs))
	fmt.Println("first and last a are one object:", line[0] == line[len(line)-1])
}
```

```output for=glyphs
a@0 b@2 r@4 a@5 c@7 a@9 d@11 a@13 b@15 r@17 a@18
characters: 11, glyph objects: 5
first and last a are one object: true
```

- Verdict: idiom. A map from the key to a pointer, and unexported fields, so code in other packages cannot change a shared glyph.
- `==` on two pointers compares the addresses, so it tests identity. `==` on two `Glyph` values compares the fields.
- Package `unique` canonicalizes comparable values. `unique.Make` returns a `Handle`, and two handles are equal when the values were equal. The node page has an example.
- A slice of `Glyph` values, not pointers, is the other choice for small immutable values. The copies sit in one array, with no pointers and no separate heap objects, and for a struct this small that can cost less memory than the shared form.
