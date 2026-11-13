// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

package filemtime

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"testing"
	"time"
)

func must(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
}

func TestCases(t *testing.T) {
	for _, c := range []struct {
		name, path string
		ns, want   int64
	}{
		{"whole-second", "a.txt", 1709296496000000000, 1709296496},
		{"sub-second", "a.txt", 1709296496500000000, 1709296496},
		{"epoch", "a.txt", 0, 0},
		{"before-1970", "a.txt", -86400000000000, -86400},
		{"before-1970-sub-second", "a.txt", -1500000000, -2},
		{"after-2038", "a.txt", 4102444800000000000, 4102444800},
		{"directory", "dir", 1709296496000000000, 1709296496},
		{"name-with-spaces", "my notes.txt", 1709296496000000000, 1709296496},
	} {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), c.path)
			if c.path == "dir" {
				must(t, os.Mkdir(path, 0o755))
			} else {
				must(t, os.WriteFile(path, []byte("alpha\n"), 0o644))
			}
			tm := time.Unix(0, c.ns)
			must(t, os.Chtimes(path, tm, tm))
			if got, err := FileMtime(path); err != nil || got != c.want {
				t.Fatalf("FileMtime = %d, %v; want %d", got, err, c.want)
			}
		})
	}
}

func TestErrors(t *testing.T) {
	if _, err := FileMtime(filepath.Join(t.TempDir(), "a.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("missing: %v", err)
	}
}

func TestSymbolicLink(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644))
	must(t, os.Chtimes(filepath.Join(dir, "a.txt"), time.Unix(0, 0), time.Unix(1709296496, 0)))
	must(t, os.Symlink("a.txt", filepath.Join(dir, "link")))
	if got, err := FileMtime(filepath.Join(dir, "link")); err != nil || got != 1709296496 {
		t.Errorf("link: %d, %v", got, err)
	}
}

func TestWriteSetsThePresentTime(t *testing.T) {
	path := filepath.Join(t.TempDir(), "a.txt")
	must(t, os.WriteFile(path, []byte("alpha\n"), 0o644))
	must(t, os.Chtimes(path, time.Unix(0, 0), time.Unix(0, 0)))
	before := time.Now().Unix() - 1
	f, err := os.OpenFile(path, os.O_WRONLY|os.O_APPEND, 0)
	must(t, err)
	_, err = f.WriteString("beta\n")
	must(t, err)
	must(t, f.Close())
	if got, err := FileMtime(path); err != nil || got < before {
		t.Errorf("after write: %d, %v; want %d or more", got, err, before)
	}
}
