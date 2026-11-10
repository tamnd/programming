// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-file/.

package writetext

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"testing"
)

func names(t *testing.T, dir string) []string {
	t.Helper()
	entries, err := os.ReadDir(dir)
	if err != nil {
		t.Fatal(err)
	}
	var out []string
	for _, e := range entries {
		out = append(out, e.Name())
	}
	return out
}

func TestCases(t *testing.T) {
	none := "\x00none" // no file before the call
	cases := []struct {
		name, text, mode, before, want string
	}{
		{"new-file", "alpha\nbeta\n", Truncate, none, "alpha\nbeta\n"},
		{"append-existing", "two\n", Append, "one\n", "one\ntwo\n"},
		{"atomic-replace", "new\n", Atomic, "old\n", "new\n"},
		{"truncate-longer", "short\n", Truncate, "a much longer old line\n", "short\n"},
		{"empty-truncate", "", Truncate, "old\n", ""},
		{"empty-new", "", Truncate, none, ""},
		{"append-missing", "first\n", Append, none, "first\n"},
		{"append-no-newline", "two", Append, "one", "onetwo"},
		{"non-ascii", "naïve café\n日本語\n🙂\n", Truncate, none, "naïve café\n日本語\n🙂\n"},
		{"crlf", "one\r\ntwo\r\n", Truncate, none, "one\r\ntwo\r\n"},
		{"mixed-endings", "lf\ncrlf\r\ncr\rend", Truncate, none, "lf\ncrlf\r\ncr\rend"},
		{"backslashes", "C:\\temp\\new\n", Truncate, none, "C:\\temp\\new\n"},
		{"format-chars", "-n 100% %s\n", Truncate, none, "-n 100% %s\n"},
		{"atomic-new", "new\n", Atomic, none, "new\n"},
		{"atomic-empty", "", Atomic, "old\n", ""},
		{"atomic-non-ascii", "naïve café\n日本語\n🙂\n", Atomic, "old\n", "naïve café\n日本語\n🙂\n"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			path := filepath.Join(dir, "out.txt")
			if c.before != none {
				if err := os.WriteFile(path, []byte(c.before), 0o666); err != nil {
					t.Fatal(err)
				}
			}
			if err := WriteText(path, c.text, c.mode); err != nil {
				t.Fatal(err)
			}
			got, err := os.ReadFile(path)
			if err != nil {
				t.Fatal(err)
			}
			if string(got) != c.want {
				t.Errorf("the file holds %q, want %q", got, c.want)
			}
			if n := names(t, dir); !slices.Equal(n, []string{"out.txt"}) {
				t.Errorf("the directory holds %q", n)
			}
		})
	}
}

func TestMissingDir(t *testing.T) {
	for _, mode := range []string{Truncate, Append, Atomic} {
		t.Run("missing-dir-"+mode, func(t *testing.T) {
			dir := t.TempDir()
			err := WriteText(filepath.Join(dir, "missing", "out.txt"), "x\n", mode)
			if !errors.Is(err, fs.ErrNotExist) {
				t.Errorf("err = %v, want fs.ErrNotExist", err)
			}
			if n := names(t, dir); len(n) != 0 {
				t.Errorf("the directory holds %q", n)
			}
		})
	}
}

func TestAtomicOntoDirectory(t *testing.T) {
	dir := t.TempDir()
	path := filepath.Join(dir, "out.txt")
	if err := os.Mkdir(path, 0o777); err != nil {
		t.Fatal(err)
	}
	if err := WriteText(path, "x\n", Atomic); err == nil {
		t.Fatal("no error")
	}
	if info, err := os.Stat(path); err != nil || !info.IsDir() {
		t.Errorf("out.txt is no longer a directory: %v", err)
	}
	if n := names(t, dir); !slices.Equal(n, []string{"out.txt"}) {
		t.Errorf("the directory holds %q", n)
	}
}
