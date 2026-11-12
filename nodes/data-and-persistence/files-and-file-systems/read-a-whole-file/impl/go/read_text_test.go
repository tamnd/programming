// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

package readtext

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

var cases = map[string]string{
	"lf":                "alpha\nbeta\n",
	"paragraphs":        "first line\n\nsecond paragraph\n",
	"non-ascii":         "naïve café\n日本語\n🙂\n",
	"empty":             "",
	"no-final-newline":  "one\ntwo",
	"trailing-newlines": "end\n\n\n",
	"crlf":              "one\r\ntwo\r\n",
	"mixed-endings":     "lf\ncrlf\r\ncr\rend",
	"bom":               "\uFEFFhead\n",
	"whitespace":        "  two spaces\ttab \n",
	"large":             strings.Repeat("0123456789abcdef", 65536),
}

var invalidUTF8 = map[string][]byte{
	"invalid-byte":      []byte("ok\n\xff\xfe\n"),
	"truncated-at-end":  []byte("ok\ncaf\xc3"),
	"encoded-surrogate": []byte("a\xed\xa0\x80b\n"),
	"overlong":          []byte("\xc0\xaf\n"),
}

func write(t *testing.T, data []byte) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.txt")
	if err := os.WriteFile(path, data, 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCases(t *testing.T) {
	for name, want := range cases {
		t.Run(name, func(t *testing.T) {
			got, err := ReadText(write(t, []byte(want)))
			if err != nil || got != want {
				t.Fatalf("ReadText = %q, %v; want %q", got, err, want)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	_, err := ReadText(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v; want fs.ErrNotExist", err)
	}
}

func TestDirectory(t *testing.T) {
	if _, err := ReadText(t.TempDir()); err == nil {
		t.Fatal("err = nil; want an error for a directory")
	}
}

func TestInvalidUTF8(t *testing.T) {
	for name, data := range invalidUTF8 {
		t.Run(name, func(t *testing.T) {
			_, err := ReadText(write(t, data))
			if !errors.Is(err, ErrInvalidUTF8) {
				t.Fatalf("err = %v; want ErrInvalidUTF8", err)
			}
		})
	}
}
