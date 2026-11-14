// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

package countlines

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

var cases = []struct {
	name            string
	data            string
	newlines, lines int64
}{
	{"lf", "alpha\nbeta\ngamma\n", 3, 3},
	{"blank-lines", "first\n\n\nlast\n", 4, 4},
	{"empty", "", 0, 0},
	{"only-newline", "\n", 1, 1},
	{"no-final-newline", "one\ntwo\nthree", 2, 3},
	{"one-line-no-newline", "x", 0, 1},
	{"crlf", "one\r\ntwo\r\n", 2, 2},
	{"crlf-no-final-newline", "one\r\ntwo", 1, 2},
	{"cr-only", "one\rtwo\r", 0, 1},
	{"other-breaks", "a\vb\fc\u0085d\u2028e\n", 1, 1},
	{"invalid-utf8", "ok\n\xff\xfe\n", 2, 2},
	{"nul-bytes", "a\x00b\n\x00\n", 2, 2},
	// 100000 lines and a last line with no LF: more than one buffer of 64 KiB.
	{"many-chunks", strings.Repeat("x\n", 100000) + "end", 100000, 100001},
}

func TestCountFile(t *testing.T) {
	dir := t.TempDir()
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(dir, c.name+".txt")
			if err := os.WriteFile(path, []byte(c.data), 0o644); err != nil {
				t.Fatal(err)
			}
			newlines, lines, err := CountFile(path)
			if err != nil || newlines != c.newlines || lines != c.lines {
				t.Fatalf("CountFile = %d, %d, %v; want %d, %d", newlines, lines, err, c.newlines, c.lines)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	_, _, err := CountFile(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v; want fs.ErrNotExist", err)
	}
}
