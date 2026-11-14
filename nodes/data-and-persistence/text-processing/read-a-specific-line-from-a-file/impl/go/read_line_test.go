// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

package specificline

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strconv"
	"strings"
	"testing"
)

// numbers returns 100000 lines with the numbers 0 to 99999. Line n holds
// n - 1.
func numbers() string {
	var b strings.Builder
	for i := 0; i < 100000; i++ {
		b.WriteString(strconv.Itoa(i) + "\n")
	}
	return b.String()
}

func TestReadLine(t *testing.T) {
	many := numbers()
	cases := []struct {
		name, text string
		number     int
		want       string
		ok         bool
	}{
		{"first", "a\nb\nc\n", 1, "a", true},
		{"middle", "a\nb\nc\n", 2, "b", true},
		{"last", "a\nb\nc\n", 3, "c", true},
		{"past-the-end", "a\nb\n", 3, "", false},
		{"zero", "a\n", 0, "", false},
		{"negative", "a\n", -1, "", false},
		{"empty", "", 1, "", false},
		{"empty-line", "a\n\nb\n", 2, "", true},
		{"only-newline", "\n", 1, "", true},
		{"no-final-newline", "a\nb", 2, "b", true},
		{"crlf", "a\r\nb\r\n", 1, "a\r", true},
		{"lone-cr", "a\rb\nc\n", 2, "c", true},
		{"non-ascii", "caf\u00e9\nzo\u00eb\n", 2, "zo\u00eb", true},
		{"many-lines", many, 100000, "99999", true},
		{"many-lines-past-the-end", many, 100001, "", false},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "input.txt")
			if err := os.WriteFile(path, []byte(c.text), 0o644); err != nil {
				t.Fatal(err)
			}
			got, ok, err := ReadLine(path, c.number)
			if err != nil {
				t.Fatal(err)
			}
			if got != c.want || ok != c.ok {
				t.Errorf("got %q, %v, want %q, %v", got, ok, c.want, c.ok)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	_, _, err := ReadLine(filepath.Join(t.TempDir(), "missing.txt"), 1)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
