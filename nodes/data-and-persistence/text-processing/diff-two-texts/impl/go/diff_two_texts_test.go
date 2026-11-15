// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

package difftexts

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"strings"
	"testing"
)

type testCase struct{ name, oldText, newText, want string }

var cases = []testCase{
	{"simple", "a\nb\nc\n", "a\nc\nd\n", " a\n-b\n c\n+d\n"},
	{"change-a-line", "one\ntwo\nthree\n", "one\n2\nthree\n", " one\n-two\n+2\n three\n"},
	{"identical", "a\nb\n", "a\nb\n", " a\n b\n"},
	{"lines-of-a-poem", "The cat sat\non the mat.\nIt was flat.\n", "The cat sat\non the hat.\nIt was flat.\nThe end.\n", " The cat sat\n-on the mat.\n+on the hat.\n It was flat.\n+The end.\n"},
	{"both-empty", "", "", ""},
	{"old-empty", "", "a\nb\n", "+a\n+b\n"},
	{"new-empty", "a\nb\n", "", "-a\n-b\n"},
	{"all-different", "a\nb\n", "c\nd\n", "-a\n-b\n+c\n+d\n"},
	{"insert-at-start", "b\nc\n", "a\nb\nc\n", "+a\n b\n c\n"},
	{"delete-at-end", "a\nb\nc\n", "a\nb\n", " a\n b\n-c\n"},
	{"swap", "a\nb\n", "b\na\n", "-a\n b\n+a\n"},
	{"repeated-lines", "a\na\nb\n", "a\nb\nb\n", " a\n-a\n b\n+b\n"},
	{"empty-line", "a\n\nb\n", "a\nb\n", " a\n-\n b\n"},
	{"no-final-newline", "a\nb", "a\nb\n", " a\n b\n"},
	{"crlf", "a\r\nb\n", "a\nb\n", "-a\r\n+a\n b\n"},
	{"trailing-space", "a \nb\n", "a\nb\n", "-a \n+a\n b\n"},
	{"normalization", "caf\u00e9\n", "cafe\u0301\n", "-caf\u00e9\n+cafe\u0301\n"},
}

func TestDiffLines(t *testing.T) {
	for _, c := range cases {
		if got := DiffLines(c.oldText, c.newText); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestManyLines(t *testing.T) {
	// 1000 lines, and 10 of them change.
	var oldText, newText strings.Builder
	for i := range 1000 {
		fmt.Fprintf(&oldText, "%d\n", i)
		if i%100 == 50 {
			fmt.Fprintf(&newText, "x%d\n", i)
		} else {
			fmt.Fprintf(&newText, "%d\n", i)
		}
	}
	lines := strings.Split(DiffLines(oldText.String(), newText.String()), "\n")
	if len(lines) != 1011 || !slices.Equal(lines[49:52], []string{" 49", "-50", "+x50"}) || lines[1009] != " 999" {
		t.Errorf("got %d lines, %q, %q", len(lines), lines[49:52], lines[1009])
	}
}

func TestDiffFiles(t *testing.T) {
	dir := t.TempDir()
	c := cases[slices.IndexFunc(cases, func(c testCase) bool { return c.name == "crlf" })]
	oldPath, newPath := filepath.Join(dir, "old.txt"), filepath.Join(dir, "new.txt")
	if err := os.WriteFile(oldPath, []byte(c.oldText), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(newPath, []byte(c.newText), 0o644); err != nil {
		t.Fatal(err)
	}
	got, err := DiffFiles(oldPath, newPath)
	if err != nil || got != c.want {
		t.Errorf("got %q, %v; want %q", got, err, c.want)
	}
}

func TestNotFound(t *testing.T) {
	dir := t.TempDir()
	newPath := filepath.Join(dir, "new.txt")
	if err := os.WriteFile(newPath, []byte("a\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if _, err := DiffFiles(filepath.Join(dir, "missing.txt"), newPath); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("got %v, want fs.ErrNotExist", err)
	}
}
