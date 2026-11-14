// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-a-word-count/.

package wordcount

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

const chunk = 64 * 1024 // the size of the buffer of the reader

var cases = []struct {
	name string
	data string
	want Counts
}{
	{"simple", "one two three\n", Counts{1, 3, 14}},
	{"multiple-spaces", "  a   b  \n", Counts{1, 2, 10}},
	{"tabs-and-lines", "a\tb\nc\n", Counts{2, 3, 6}},
	{"empty", "", Counts{0, 0, 0}},
	{"only-white-space", " \n\t\n", Counts{2, 0, 4}},
	{"no-final-newline", "one two", Counts{0, 2, 7}},
	{"crlf", "a b\r\nc\r\n", Counts{2, 3, 8}},
	{"non-ascii", "na\u00efve caf\u00e9\n", Counts{1, 2, 13}},
	{"unicode-spaces", "a\u00a0b\u3000c\u2028d\n", Counts{1, 4, 13}},
	{"not-white-space", "a\u200bb\u00adc\ufeffd\n", Counts{1, 1, 13}},
	{"control-separators", "a\x1cb\x1fc\n", Counts{1, 1, 6}},
	{"nel", "a\u0085b\n", Counts{1, 2, 5}},
	// 100000 lines: more than one buffer of 64 KiB.
	{"many-lines", strings.Repeat("a b\n", 100000), Counts{100000, 200000, 400000}},
	// The buffer ends inside the 3 bytes of U+3000.
	{"chunk-boundary", strings.Repeat("x", chunk-1) + "\u3000y\n", Counts{1, 2, chunk + 4}},
}

// The bytes of files that are not valid UTF-8.
var invalid = []struct{ name, data string }{
	{"invalid-bytes", "ok\xff\xfe\n"},
	{"truncated", "ok \xe2\x82"},
	{"surrogate", "a\xed\xa0\x80b\n"},
}

func write(t *testing.T, data string) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.txt")
	if err := os.WriteFile(path, []byte(data), 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCountFile(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := CountFile(write(t, c.data))
			if err != nil || got != c.want {
				t.Fatalf("CountFile = %+v, %v; want %+v", got, err, c.want)
			}
		})
	}
}

func TestInvalidUTF8(t *testing.T) {
	for _, c := range invalid {
		t.Run(c.name, func(t *testing.T) {
			if _, err := CountFile(write(t, c.data)); !errors.Is(err, ErrInvalidUTF8) {
				t.Fatalf("err = %v; want ErrInvalidUTF8", err)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	_, err := CountFile(filepath.Join(t.TempDir(), "missing.txt"))
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v; want fs.ErrNotExist", err)
	}
}
