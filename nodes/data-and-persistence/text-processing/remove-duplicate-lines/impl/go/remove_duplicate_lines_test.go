// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-duplicate-lines/.

package dedup

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"strconv"
	"strings"
	"testing"
)

// manyLines returns 100000 lines with 1000 different lines, and the result.
func manyLines() (string, []string) {
	var b strings.Builder
	for i := range 100000 {
		b.WriteString(strconv.Itoa(i%1000) + "\n")
	}
	want := make([]string, 1000)
	for i := range want {
		want[i] = strconv.Itoa(i)
	}
	return b.String(), want
}

var manyData, manyWant = manyLines()

var cases = []struct {
	name string
	data string
	want []string
}{
	{"simple", "b\na\nb\nc\na\n", []string{"b", "a", "c"}},
	{"adjacent", "a\na\nb\n", []string{"a", "b"}},
	{"no-duplicates", "c\nb\na\n", []string{"c", "b", "a"}},
	{"empty", "", []string{}},
	{"empty-lines", "\na\n\n\n", []string{"", "a"}},
	{"all-same", "x\nx\nx\n", []string{"x"}},
	{"no-final-newline", "a\nb\na", []string{"a", "b"}},
	{"crlf", "a\r\na\n", []string{"a\r", "a"}},
	{"case", "A\na\nA\n", []string{"A", "a"}},
	{"whitespace", "a \na\n a\na \n", []string{"a ", "a", " a"}},
	// U+00E9, then e and U+0301: the same letter, but not the same code points.
	{"normalization", "\u00e9\ne\u0301\n\u00e9\n", []string{"\u00e9", "e\u0301"}},
	{"many-lines", manyData, manyWant},
}

func TestRemoveDuplicateLines(t *testing.T) {
	dir := t.TempDir()
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(dir, c.name+".txt")
			if err := os.WriteFile(path, []byte(c.data), 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := RemoveDuplicateLines(path)
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
	_, err := RemoveDuplicateLines(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
