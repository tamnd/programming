// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-word-frequencies/.

package wordfreq

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
	n    int
	want []WordCount
}{
	{"simple", "the cat and the hat\n", 2, []WordCount{{"the", 2}, {"and", 1}}},
	{"case", "The THE the tHe\n", 5, []WordCount{{"the", 4}}},
	{"punctuation", "it's well-dressed, isn't it?\n", 10,
		[]WordCount{{"it", 2}, {"dressed", 1}, {"isn", 1}, {"s", 1}, {"t", 1}, {"well", 1}}},
	{"empty", "", 3, []WordCount{}},
	{"no-letters", "123 ... 42\n-- !!\n", 3, []WordCount{}},
	{"ties", "b a c b a c\n", 2, []WordCount{{"a", 2}, {"b", 2}}},
	{"digits", "abc123def 42 x9\n", 10, []WordCount{{"abc", 1}, {"def", 1}, {"x", 1}}},
	{"n-zero", "the cat and the hat\n", 0, []WordCount{}},
	{"n-above-count", "x y x\n", 10, []WordCount{{"x", 2}, {"y", 1}}},
	{"crlf", "one two\r\ntwo\r\n", 5, []WordCount{{"two", 2}, {"one", 1}}},
	{"non-ascii", "caf\u00e9 na\u00efve cafe\n", 10, []WordCount{{"caf", 1}, {"cafe", 1}, {"na", 1}, {"ve", 1}}},
	{"leading-separators", "  ...hello, world. Hello\n", 5, []WordCount{{"hello", 2}, {"world", 1}}},
	{"no-final-newline", "dog cat dog", 5, []WordCount{{"dog", 2}, {"cat", 1}}},
	{"not-utf8", "ok \xff\xfe ok\n", 5, []WordCount{{"ok", 2}}},
	{"many-lines", strings.Repeat("a b a\n", 100000), 3, []WordCount{{"a", 200000}, {"b", 100000}}},
	// One line of 100000 words: longer than the buffer of 64 KiB.
	{"long-line", strings.Repeat("word ", 100000), 3, []WordCount{{"word", 100000}}},
}

func TestWordFrequencies(t *testing.T) {
	dir := t.TempDir()
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(dir, c.name+".txt")
			if err := os.WriteFile(path, []byte(c.data), 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := WordFrequencies(path, c.n)
			if err != nil {
				t.Fatal(err)
			}
			if !slices.Equal(got, c.want) {
				t.Errorf("got %v, want %v", got, c.want)
			}
		})
	}
}

func TestNotFound(t *testing.T) {
	_, err := WordFrequencies(filepath.Join(t.TempDir(), "missing.txt"), 3)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
