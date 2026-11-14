// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-the-first-and-last-lines-of-a-file/.

package firstandlast

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"reflect"
	"strconv"
	"strings"
	"testing"
)

// numbers returns the numbers from and up to to, but not to, one on each line.
func numbers(from, to int) []string {
	s := []string{}
	for i := from; i < to; i++ {
		s = append(s, strconv.Itoa(i))
	}
	return s
}

func text(lines []string) string {
	var b strings.Builder
	for _, l := range lines {
		b.WriteString(l + "\n")
	}
	return b.String()
}

func TestFirstAndLastLines(t *testing.T) {
	cases := []struct {
		name, text  string
		count       int
		first, last []string
	}{
		{"two", "a\nb\nc\nd\ne\n", 2, []string{"a", "b"}, []string{"d", "e"}},
		{"one", "first\nmiddle\nlast\n", 1, []string{"first"}, []string{"last"}},
		{"ten", text(numbers(1, 13)), 10, numbers(1, 11), numbers(3, 13)},
		{"empty", "", 2, []string{}, []string{}},
		{"zero", "a\nb\n", 0, []string{}, []string{}},
		{"short", "a\nb\n", 5, []string{"a", "b"}, []string{"a", "b"}},
		{"exact", "a\nb\n", 2, []string{"a", "b"}, []string{"a", "b"}},
		{"overlap", "a\nb\nc\n", 2, []string{"a", "b"}, []string{"b", "c"}},
		{"one-line", "a\n", 1, []string{"a"}, []string{"a"}},
		{"empty-lines", "\n\na\n\n", 1, []string{""}, []string{""}},
		{"only-newline", "\n", 1, []string{""}, []string{""}},
		{"no-final-newline", "a\nb\nc", 1, []string{"a"}, []string{"c"}},
		{"crlf", "a\r\nb\r\n", 1, []string{"a\r"}, []string{"b\r"}},
		{"non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 1, []string{"caf\u00e9"}, []string{"zo\u00eb"}},
		// 100000 lines with the numbers 0 to 99999.
		{"many-lines", text(numbers(0, 100000)), 3, []string{"0", "1", "2"}, []string{"99997", "99998", "99999"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "input.txt")
			if err := os.WriteFile(path, []byte(c.text), 0o644); err != nil {
				t.Fatal(err)
			}
			first, last, err := FirstAndLastLines(path, c.count)
			if err != nil {
				t.Fatal(err)
			}
			if !reflect.DeepEqual(first, c.first) || !reflect.DeepEqual(last, c.last) {
				t.Errorf("got %q and %q, want %q and %q", first, last, c.first, c.last)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	_, _, err := FirstAndLastLines(filepath.Join(t.TempDir(), "missing.txt"), 1)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
