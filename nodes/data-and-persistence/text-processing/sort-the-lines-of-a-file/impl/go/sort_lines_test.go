// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-the-lines-of-a-file/.

package sortlines

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"strings"
	"testing"
)

var cases = []struct {
	name string
	data string
	want []string
}{
	{"simple", "pear\napple\nfig\n", []string{"apple", "fig", "pear"}},
	{"case", "b\nB\na\nA\n", []string{"A", "B", "a", "b"}},
	{"numbers", "10\n9\n100\n", []string{"10", "100", "9"}},
	{"empty", "", []string{}},
	{"one-empty-line", "\n", []string{""}},
	{"empty-lines", "b\n\na\n\n", []string{"", "", "a", "b"}},
	{"duplicates", "b\na\nb\n", []string{"a", "b", "b"}},
	{"no-final-newline", "b\na", []string{"a", "b"}},
	{"crlf", "b\r\na\r\n", []string{"a\r", "b\r"}},
	{"prefix", "ab\na\n", []string{"a", "ab"}},
	{"whitespace", "b\n a\n\tc\n", []string{"\tc", " a", "b"}},
	{"non-ascii", "\u00e9\nz\ne\n", []string{"e", "z", "\u00e9"}},
	// U+FF61 is below U+1F600, but its UTF-16 code unit is above 0xD83D.
	{"supplementary", "\U0001f600\n\uff61\n", []string{"\uff61", "\U0001f600"}},
	{"other-line-breaks", "b\u2028a\nb\x1ca\nb\x0ca\n", []string{"b\x0ca", "b\x1ca", "b\u2028a"}},
	{"many-lines", strings.Repeat("b\na\n", 50000), aThenB(50000)},
}

// aThenB returns n lines "a" and then n lines "b".
func aThenB(n int) []string {
	lines := make([]string, 2*n)
	for i := range lines {
		lines[i] = "a"
		if i >= n {
			lines[i] = "b"
		}
	}
	return lines
}

func TestSortLines(t *testing.T) {
	dir := t.TempDir()
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(dir, c.name+".txt")
			if err := os.WriteFile(path, []byte(c.data), 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := SortLines(path)
			if err != nil {
				t.Fatal(err)
			}
			if !slices.Equal(got, c.want) {
				t.Errorf("got %q, want %q", got, c.want)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	_, err := SortLines(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
