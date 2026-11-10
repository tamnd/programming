// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-file-line-by-line/.

package linestats

import (
	"bufio"
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func write(t *testing.T, data string) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.txt")
	if err := os.WriteFile(path, []byte(data), 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCases(t *testing.T) {
	cases := []struct {
		name           string
		data           string
		lines, longest int
	}{
		{"lf", "alpha\nbeta\ngamma\n", 3, 5},
		{"blank-lines", "first\n\n\nlast line\n", 4, 9},
		{"empty", "", 0, 0},
		{"only-newline", "\n", 1, 0},
		{"no-final-newline", "one\ntwo\nthree", 3, 5},
		{"crlf", "one\r\ntwo\r\nthree\r\n", 3, 5},
		{"crlf-no-final-newline", "one\r\ntwo\r\nthree", 3, 5},
		{"mixed-endings", "one\r\ntwo\nthree\r\n", 3, 5},
		{"whitespace", "  two spaces\ttab \n", 1, 17},
		{"backslashes", `C:\temp\new` + "\n", 1, 11},
		{"long-line", strings.Repeat("x", 1<<20) + "\nend\n", 2, 1 << 20},
		{"non-ascii", "naïve café\n日本語のテキスト\n🙂🙂🙂🙂🙂🙂\n", 3, 10},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			lines, longest, err := LineStats(write(t, c.data))
			if err != nil || lines != c.lines || longest != c.longest {
				t.Errorf("LineStats = %d, %d, %v; want %d, %d, nil", lines, longest, err, c.lines, c.longest)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	_, _, err := LineStats(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("err = %v, want fs.ErrNotExist", err)
	}
}

func TestInvalidUTF8(t *testing.T) {
	for name, data := range map[string]string{
		"invalid-byte":      "ok\n\xff\xfe\n",
		"truncated-at-end":  "ok\ncaf\xc3",
		"encoded-surrogate": "a\xed\xa0\x80b\n",
		"overlong":          "\xc0\xaf\n",
	} {
		t.Run(name, func(t *testing.T) {
			if _, _, err := LineStats(write(t, data)); !errors.Is(err, ErrInvalidUTF8) {
				t.Errorf("err = %v, want ErrInvalidUTF8", err)
			}
		})
	}
}

// The default buffer of bufio.Scanner stops at a line of 64 KiB. LineStats raises the limit, and a longer line is still an error.
func TestLineTooLong(t *testing.T) {
	if _, _, err := LineStats(write(t, strings.Repeat("x", MaxLine+1))); !errors.Is(err, bufio.ErrTooLong) {
		t.Errorf("err = %v, want bufio.ErrTooLong", err)
	}
}
