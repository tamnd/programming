// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/implement-a-string-form-for-a-type/.

package color

import (
	"fmt"
	"slices"
	"testing"
)

var cases = []struct {
	name                string
	reds, greens, blues []int
	want                []string
}{
	{"orange", []int{255}, []int{128}, []int{0}, []string{"#FF8000"}},
	{"three", []int{255, 0, 0}, []int{0, 255, 0}, []int{0, 0, 255}, []string{"#FF0000", "#00FF00", "#0000FF"}},
	{"mixed", []int{18, 171}, []int{52, 205}, []int{86, 239}, []string{"#123456", "#ABCDEF"}},
	{"empty", []int{}, []int{}, []int{}, []string{}},
	{"single", []int{1}, []int{2}, []int{3}, []string{"#010203"}},
	{"zero", []int{0}, []int{0}, []int{0}, []string{"#000000"}},
	{"max", []int{255}, []int{255}, []int{255}, []string{"#FFFFFF"}},
	{"padding", []int{10, 0}, []int{11, 15}, []int{12, 16}, []string{"#0A0B0C", "#000F10"}},
}

func TestFormatColors(t *testing.T) {
	for _, c := range cases {
		got, err := FormatColors(c.reds, c.greens, c.blues)
		if err != nil || !slices.Equal(got, c.want) {
			t.Errorf("case %s: got %v, %v; want %v", c.name, got, err, c.want)
		}
	}
}

func TestForms(t *testing.T) {
	c, err := New(255, 128, 0)
	if err != nil {
		t.Fatal(err)
	}
	checks := []struct{ got, want string }{
		{fmt.Sprint(c), "#FF8000"},
		{fmt.Sprintf("%v and %s", c, c), "#FF8000 and #FF8000"},
		{fmt.Sprint([]Color{c, {}}), "[#FF8000 #000000]"},
		{fmt.Sprintf("%#v", c), "color.New(255, 128, 0)"},
	}
	for _, ch := range checks {
		if ch.got != ch.want {
			t.Errorf("got %q, want %q", ch.got, ch.want)
		}
	}
}

func TestBadValue(t *testing.T) {
	if _, err := New(256, 0, 0); err == nil {
		t.Error("New(256, 0, 0) gave no error")
	}
}
