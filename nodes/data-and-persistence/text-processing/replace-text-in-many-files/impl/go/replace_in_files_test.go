// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-text-in-many-files/.

package replaceinfiles

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"runtime"
	"slices"
	"strconv"
	"strings"
	"testing"
)

type file struct{ name, before, after string }

func TestReplaceInFiles(t *testing.T) {
	cases := []struct {
		name     string
		old, new string
		files    []file
		want     []int
	}{
		{"rosetta", "Goodbye London!", "Hello New York!", []file{
			{"a.txt", "Goodbye London!\n", "Hello New York!\n"},
			{"b.txt", "Goodbye London! Goodbye London!\nGoodbye\n", "Hello New York! Hello New York!\nGoodbye\n"},
		}, []int{1, 2}},
		{"some-files-match", "dog", "fox", []file{{"a.txt", "cat\n", "cat\n"}, {"b.txt", "dog\ncat dog\n", "fox\ncat fox\n"}}, []int{0, 2}},
		{"one-file", "1", "2", []file{{"a.txt", "x = 1\ny = 1\n", "x = 2\ny = 2\n"}}, []int{2}},
		{"no-files", "a", "b", nil, []int{}},
		{"no-match", "x", "y", []file{{"a.txt", "abc\n", "abc\n"}}, []int{0}},
		{"empty-file", "a", "b", []file{{"a.txt", "", ""}}, []int{0}},
		{"delete", "-", "", []file{{"a.txt", "a-b-c\n", "abc\n"}}, []int{2}},
		{"whole-file", "abc", "", []file{{"a.txt", "abc", ""}}, []int{1}},
		{"new-contains-old", "a", "aa", []file{{"a.txt", "a a\n", "aa aa\n"}}, []int{2}},
		{"overlapping", "aa", "b", []file{{"a.txt", "aaa\n", "ba\n"}}, []int{1}},
		{"adjacent", "aa", "b", []file{{"a.txt", "aaaa\n", "bb\n"}}, []int{2}},
		{"special-chars", "1.5", `\&$1 $& [x]`, []file{{"a.txt", "1.5 and 125\n", `\&$1 $& [x]` + " and 125\n"}}, []int{1}},
		{"glob-chars", "*.txt", "?", []file{{"a.txt", "a.txt *.txt\n", "a.txt ?\n"}}, []int{1}},
		{"across-lines", "end\nstart", "end start", []file{{"a.txt", "end\nstart\n", "end start\n"}}, []int{1}},
		{"lf-to-crlf", "\n", "\r\n", []file{{"a.txt", "a\nb\n", "a\r\nb\r\n"}}, []int{2}},
		{"crlf", "a", "c", []file{{"a.txt", "a\r\nb\r\n", "c\r\nb\r\n"}}, []int{1}},
		{"no-final-newline", "b", "c", []file{{"a.txt", "a b", "a c"}}, []int{1}},
		{"non-ascii", "\u00e9", "e", []file{{"a.txt", "caf\u00e9, cafe\n", "cafe, cafe\n"}}, []int{1}},
		{"case-sensitive", "hello", "bye", []file{{"a.txt", "Hello hello\n", "Hello bye\n"}}, []int{1}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			paths := []string{}
			for _, f := range c.files {
				p := filepath.Join(dir, f.name)
				if err := os.WriteFile(p, []byte(f.before), 0o644); err != nil {
					t.Fatal(err)
				}
				paths = append(paths, p)
			}
			got, err := ReplaceInFiles(paths, c.old, c.new)
			if err != nil || !slices.Equal(got, c.want) {
				t.Fatalf("got %v, %v; want %v", got, err, c.want)
			}
			for _, f := range c.files {
				data, err := os.ReadFile(filepath.Join(dir, f.name))
				if err != nil || string(data) != f.after {
					t.Errorf("%s after: got %q, %v; want %q", f.name, data, err, f.after)
				}
			}
			// No temporary file stays in the directory.
			if entries, _ := os.ReadDir(dir); len(entries) != len(c.files) {
				t.Errorf("%d files in the directory, want %d", len(entries), len(c.files))
			}
		})
	}
}

func lines(prefix string) string {
	var b strings.Builder
	for i := range 100000 {
		b.WriteString(prefix + strconv.Itoa(i) + "\n")
	}
	return b.String()
}

func TestManyLines(t *testing.T) {
	path := filepath.Join(t.TempDir(), "many.txt")
	if err := os.WriteFile(path, []byte(lines("line ")), 0o644); err != nil {
		t.Fatal(err)
	}
	got, err := ReplaceInFiles([]string{path}, "line ", "L")
	if err != nil || !slices.Equal(got, []int{100000}) {
		t.Fatalf("got %v, %v", got, err)
	}
	if data, _ := os.ReadFile(path); string(data) != lines("L") {
		t.Error("the file after the call is wrong")
	}
}

func TestKeepsThePermissions(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("no Unix permissions")
	}
	path := filepath.Join(t.TempDir(), "a.txt")
	if err := os.WriteFile(path, []byte("a\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	if err := os.Chmod(path, 0o640); err != nil {
		t.Fatal(err)
	}
	if _, err := ReplaceInFiles([]string{path}, "a", "b"); err != nil {
		t.Fatal(err)
	}
	if info, _ := os.Stat(path); info.Mode().Perm() != 0o640 {
		t.Errorf("mode %v, want 0640", info.Mode().Perm())
	}
}

func TestNoMatchDoesNotWrite(t *testing.T) {
	path := filepath.Join(t.TempDir(), "a.txt")
	if err := os.WriteFile(path, []byte("a\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	before, _ := os.Stat(path)
	if got, err := ReplaceInFiles([]string{path}, "x", "y"); err != nil || !slices.Equal(got, []int{0}) {
		t.Fatalf("got %v, %v", got, err)
	}
	if after, _ := os.Stat(path); !os.SameFile(before, after) {
		t.Error("the file was written again")
	}
}

func TestErrors(t *testing.T) {
	dir := t.TempDir()
	if _, err := ReplaceInFiles([]string{filepath.Join(dir, "missing.txt")}, "a", "b"); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("not-found: got %v", err)
	}
	path := filepath.Join(dir, "a.txt")
	if err := os.WriteFile(path, []byte("a\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if _, err := ReplaceInFiles([]string{path}, "", "b"); !errors.Is(err, ErrEmptyOld) {
		t.Errorf("empty-old: got %v", err)
	}
	if data, _ := os.ReadFile(path); string(data) != "a\n" {
		t.Errorf("empty-old changed the file: %q", data)
	}
}

func TestStop(t *testing.T) {
	dir := t.TempDir()
	for _, name := range []string{"a.txt", "c.txt"} {
		if err := os.WriteFile(filepath.Join(dir, name), []byte("a\n"), 0o644); err != nil {
			t.Fatal(err)
		}
	}
	paths := []string{filepath.Join(dir, "a.txt"), filepath.Join(dir, "missing.txt"), filepath.Join(dir, "c.txt")}
	if _, err := ReplaceInFiles(paths, "a", "b"); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("got %v", err)
	}
	for name, want := range map[string]string{"a.txt": "b\n", "c.txt": "a\n"} {
		if data, _ := os.ReadFile(filepath.Join(dir, name)); string(data) != want {
			t.Errorf("%s: got %q, want %q", name, data, want)
		}
	}
}
