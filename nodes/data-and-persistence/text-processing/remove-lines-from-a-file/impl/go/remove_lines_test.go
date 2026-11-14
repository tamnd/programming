// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

package removelines

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"runtime"
	"strconv"
	"strings"
	"testing"
)

func numbers() string {
	var b strings.Builder
	for i := range 100000 {
		b.WriteString(strconv.Itoa(i) + "\n")
	}
	return b.String()
}

func TestRemoveLines(t *testing.T) {
	many := numbers()
	cases := []struct {
		name         string
		text         string
		start, count int
		want         int
		after        string
	}{
		{"first-two", "a\nb\nc\nd\n", 1, 2, 2, "c\nd\n"},
		{"middle", "a\nb\nc\nd\n", 2, 2, 2, "a\nd\n"},
		{"last", "a\nb\nc\n", 3, 1, 1, "a\nb\n"},
		{"past-the-end", "a\nb\n", 3, 1, 0, "a\nb\n"},
		{"partly-past-the-end", "a\nb\nc\n", 2, 5, 2, "a\n"},
		{"count-zero", "a\nb\n", 1, 0, 0, "a\nb\n"},
		{"all-lines", "a\nb\n", 1, 2, 2, ""},
		{"empty", "", 1, 1, 0, ""},
		{"empty-line", "a\n\nb\n", 2, 1, 1, "a\nb\n"},
		{"no-final-newline", "a\nb", 1, 1, 1, "b"},
		{"remove-incomplete-line", "a\nb", 2, 1, 1, "a\n"},
		{"crlf", "a\r\nb\r\nc\r\n", 2, 1, 1, "a\r\nc\r\n"},
		{"lone-cr", "a\rb\nc\n", 1, 1, 1, "c\n"},
		{"non-ascii", "caf\u00e9\nna\u00efve\nzo\u00eb\n", 2, 1, 1, "caf\u00e9\nzo\u00eb\n"},
		{"large-count", "a\nb\n", 2, 2147483647, 1, "a\n"},
		// 100000 lines with the numbers 0 to 99999. Lines 2 to 99999 go.
		{"many-lines", many, 2, 99998, 99998, "0\n99999\n"},
		// A line longer than the 4096-byte buffer of bufio.Reader.
		{"long-line", "a\n" + strings.Repeat("x", 10000) + "\nb\n", 2, 1, 1, "a\nb\n"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			path := filepath.Join(dir, "input.txt")
			if err := os.WriteFile(path, []byte(c.text), 0o644); err != nil {
				t.Fatal(err)
			}
			got, err := RemoveLines(path, c.start, c.count)
			if err != nil || got != c.want {
				t.Fatalf("RemoveLines(%d, %d) = %d, %v; want %d", c.start, c.count, got, err, c.want)
			}
			b, err := os.ReadFile(path)
			if err != nil || string(b) != c.after {
				t.Fatalf("file after = %q, %v; want %q", b, err, c.after)
			}
			// No temporary file stays in the directory.
			if entries, _ := os.ReadDir(dir); len(entries) != 1 {
				t.Fatalf("%d files in the directory, want 1", len(entries))
			}
		})
	}
}

func TestNoChangeKeepsTheFile(t *testing.T) {
	path := filepath.Join(t.TempDir(), "input.txt")
	if err := os.WriteFile(path, []byte("a\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	before, _ := os.Stat(path)
	if got, err := RemoveLines(path, 2, 1); err != nil || got != 0 {
		t.Fatalf("RemoveLines = %d, %v; want 0", got, err)
	}
	after, _ := os.Stat(path)
	if !os.SameFile(before, after) {
		t.Fatal("the file was written again")
	}
}

func TestKeepsThePermissions(t *testing.T) {
	if runtime.GOOS == "windows" {
		t.Skip("POSIX permission bits")
	}
	path := filepath.Join(t.TempDir(), "input.txt")
	if err := os.WriteFile(path, []byte("a\nb\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.Chmod(path, 0o640); err != nil {
		t.Fatal(err)
	}
	if _, err := RemoveLines(path, 1, 1); err != nil {
		t.Fatal(err)
	}
	if info, _ := os.Stat(path); info.Mode().Perm() != 0o640 {
		t.Fatalf("mode %v, want 0640", info.Mode().Perm())
	}
}

func TestNotFound(t *testing.T) {
	_, err := RemoveLines(filepath.Join(t.TempDir(), "missing.txt"), 1, 1)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v, want fs.ErrNotExist", err)
	}
}

func TestBadRange(t *testing.T) {
	for _, r := range [][2]int{{0, 1}, {1, -1}} {
		path := filepath.Join(t.TempDir(), "input.txt")
		if err := os.WriteFile(path, []byte("a\nb\n"), 0o644); err != nil {
			t.Fatal(err)
		}
		if _, err := RemoveLines(path, r[0], r[1]); !errors.Is(err, ErrBadRange) {
			t.Fatalf("RemoveLines(%d, %d) err = %v, want ErrBadRange", r[0], r[1], err)
		}
		if b, _ := os.ReadFile(path); string(b) != "a\nb\n" {
			t.Fatalf("file after = %q", b)
		}
	}
}
