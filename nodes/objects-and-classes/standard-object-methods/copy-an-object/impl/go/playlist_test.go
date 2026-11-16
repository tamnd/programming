// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-an-object/.

package playlist

import (
	"slices"
	"testing"
)

var cases = []struct {
	name  string
	songs []string
	index int
	title string
	want  [][]string
}{
	{"middle", []string{"Intro", "Song", "Outro"}, 1, "Hit",
		[][]string{{"Intro", "Hit", "Outro"}, {"Intro", "Hit", "Outro"}, {"Intro", "Song", "Outro"}}},
	{"two", []string{"A", "B"}, 0, "C", [][]string{{"C", "B"}, {"C", "B"}, {"A", "B"}}},
	{"single", []string{"Solo"}, 0, "Duet", [][]string{{"Duet"}, {"Duet"}, {"Solo"}}},
	{"first", []string{"a", "b", "c"}, 0, "z", [][]string{{"z", "b", "c"}, {"z", "b", "c"}, {"a", "b", "c"}}},
	{"last", []string{"a", "b", "c"}, 2, "z", [][]string{{"a", "b", "z"}, {"a", "b", "z"}, {"a", "b", "c"}}},
	{"same-title", []string{"a", "b"}, 1, "b", [][]string{{"a", "b"}, {"a", "b"}, {"a", "b"}}},
	{"duplicates", []string{"x", "x", "x"}, 1, "y", [][]string{{"x", "y", "x"}, {"x", "y", "x"}, {"x", "x", "x"}}},
}

func TestCopyAndChange(t *testing.T) {
	for _, c := range cases {
		got, err := CopyAndChange(c.songs, c.index, c.title)
		if err != nil || !slices.EqualFunc(got, c.want, slices.Equal[[]string]) {
			t.Errorf("case %s: got %v, %v; want %v", c.name, got, err, c.want)
		}
	}
}

func TestCopiesDiffer(t *testing.T) {
	original := Playlist{Name: "mix", Songs: []string{"a"}}
	shallow := original
	deep := original.DeepCopy()
	shallow.Name = "other"
	if original.Name != "mix" {
		t.Error("a change of the copy's name changed the original")
	}
	if &shallow.Songs[0] != &original.Songs[0] || &deep.Songs[0] == &original.Songs[0] {
		t.Error("the shallow copy must share the array and the deep copy must not")
	}
}

func TestBadIndex(t *testing.T) {
	if _, err := CopyAndChange([]string{"a"}, 1, "b"); err == nil {
		t.Error("index 1 of one song gave no error")
	}
}
