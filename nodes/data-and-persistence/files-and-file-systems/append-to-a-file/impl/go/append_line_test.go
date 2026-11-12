// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

package appendline

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"strings"
	"sync"
	"testing"
)

type appendCase struct {
	old        *string // nil when no file is there
	line, want string
}

func ptr(s string) *string { return &s }

var cases = map[string]appendCase{
	"new-file":            {nil, "first", "first\n"},
	"append-existing":     {ptr("one\n"), "two", "one\ntwo\n"},
	"new-file-empty-line": {nil, "", "\n"},
	"empty-line":          {ptr("a\n"), "", "a\n\n"},
	"empty-file":          {ptr(""), "x", "x\n"},
	"no-final-newline":    {ptr("one"), "two", "onetwo\n"},
	"crlf-file":           {ptr("one\r\n"), "two", "one\r\ntwo\n"},
	"whitespace":          {ptr(""), "  two spaces\ttab ", "  two spaces\ttab \n"},
	"non-ascii":           {ptr("naïve\n"), "café 日本語 🙂", "naïve\ncafé 日本語 🙂\n"},
}

func TestCases(t *testing.T) {
	for name, c := range cases {
		t.Run(name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), "log.txt")
			if c.old != nil {
				if err := os.WriteFile(path, []byte(*c.old), 0o644); err != nil {
					t.Fatal(err)
				}
			}
			if err := AppendLine(path, c.line); err != nil {
				t.Fatal(err)
			}
			got, err := os.ReadFile(path)
			if err != nil || string(got) != c.want {
				t.Fatalf("file = %q, %v; want %q", got, err, c.want)
			}
		})
	}
}

func TestMissingDir(t *testing.T) {
	err := AppendLine(filepath.Join(t.TempDir(), "missing", "log.txt"), "x")
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v; want fs.ErrNotExist", err)
	}
}

func TestInvalidLine(t *testing.T) {
	for _, line := range []string{"a\nb", "a\rb"} {
		path := filepath.Join(t.TempDir(), "log.txt")
		if err := AppendLine(path, line); !errors.Is(err, ErrInvalidLine) {
			t.Fatalf("AppendLine(%q) = %v; want ErrInvalidLine", line, err)
		}
		if _, err := os.Stat(path); !errors.Is(err, fs.ErrNotExist) {
			t.Fatalf("the file was made for %q", line)
		}
	}
}

func text(w, i int) string {
	return fmt.Sprintf("writer %d line %03d ", w, i) + strings.Repeat("x", 40)
}

func TestManyWriters(t *testing.T) {
	path := filepath.Join(t.TempDir(), "log.txt")
	const writers, lines = 8, 250
	var wg sync.WaitGroup
	errs := make(chan error, writers)
	for w := range writers {
		wg.Add(1)
		go func() {
			defer wg.Done()
			for i := range lines {
				if err := AppendLine(path, text(w, i)); err != nil {
					errs <- err
					return
				}
			}
		}()
	}
	wg.Wait()
	close(errs)
	for err := range errs {
		t.Fatal(err)
	}
	data, err := os.ReadFile(path)
	if err != nil {
		t.Fatal(err)
	}
	got := strings.Split(strings.TrimSuffix(string(data), "\n"), "\n")
	var want []string
	for w := range writers {
		for i := range lines {
			want = append(want, text(w, i))
		}
	}
	slices.Sort(got)
	slices.Sort(want)
	if !slices.Equal(got, want) {
		t.Fatalf("got %d lines; want the %d lines of the writers", len(got), len(want))
	}
}
