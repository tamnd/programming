// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

// Package playlist copies a struct with a slice field in two ways.
package playlist

import (
	"fmt"
	"slices"
)

// Playlist is a struct with a slice field.
type Playlist struct {
	Name  string
	Songs []string
}

// DeepCopy returns a copy with its own slice. Assignment alone copies the
// slice header, so the copy would share the array of the original.
func (p Playlist) DeepCopy() Playlist {
	c := p
	c.Songs = slices.Clone(p.Songs)
	return c
}

// CopyAndChange changes one song through the original and returns the songs
// of the original, of a shallow copy and of a deep copy.
func CopyAndChange(songs []string, index int, title string) ([][]string, error) {
	if index < 0 || index >= len(songs) {
		return nil, fmt.Errorf("no song at index %d", index)
	}
	original := Playlist{Name: "mix", Songs: slices.Clone(songs)}
	shallow := original // a shallow copy: the fields are copied, and Songs shares the array
	deep := original.DeepCopy()
	original.Songs[index] = title
	return [][]string{slices.Clone(original.Songs), slices.Clone(shallow.Songs), slices.Clone(deep.Songs)}, nil
}
