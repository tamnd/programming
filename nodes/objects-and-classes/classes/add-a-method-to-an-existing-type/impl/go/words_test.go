// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

package words

import "testing"

var cases = []struct {
	name string
	text string
	want int
}{
	{"three-words", "the quick fox", 3},
	{"one-word", "hello", 1},
	{"punctuation", "Hello, world! How are you?", 5},
	{"empty", "", 0},
	{"blank", " \t\n ", 0},
	{"mixed-whitespace", "  two\twords\r\nand three  ", 4},
	{"non-ascii", "naïve café 東京", 3},
}

func TestWordCount(t *testing.T) {
	for _, c := range cases {
		if got := WordCount(c.text); got != c.want {
			t.Errorf("%s: got %d, want %d", c.name, got, c.want)
		}
	}
}

func TestTextIsAString(t *testing.T) {
	var x Text = "one two"
	if x.WordCount() != 2 || string(x)+"!" != "one two!" || len(x) != 7 {
		t.Fatal("Text does not behave like a string")
	}
}
