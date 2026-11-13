// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

package filesize

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
)

func must(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
}

func TestCases(t *testing.T) {
	for _, c := range []struct {
		name, path, data string
		size             int64
	}{
		{"text", "a.txt", "alpha\n", 6},
		{"non-ascii-text", "a.txt", "café\n", 6},
		{"empty-file", "a.txt", "", 0},
		{"binary-data", "a.bin", "\x00\r\n\xc3\xbf\x00", 6},
		{"crlf-line-ends", "a.txt", "a\r\nb\r\n", 6},
		{"name-with-spaces", "my notes.txt", "alpha\n", 6},
		{"non-ascii-name", "café 日本.txt", "alpha\n", 6},
	} {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), c.path)
			must(t, os.WriteFile(path, []byte(c.data), 0o644))
			got, err := FileSize(path)
			if err != nil || got != c.size {
				t.Fatalf("FileSize = %d, %v; want %d", got, err, c.size)
			}
		})
	}
}

func TestErrors(t *testing.T) {
	dir := t.TempDir()
	if _, err := FileSize(filepath.Join(dir, "a.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("missing: %v", err)
	}
	must(t, os.Mkdir(filepath.Join(dir, "dir"), 0o755))
	if _, err := FileSize(filepath.Join(dir, "dir")); !errors.Is(err, syscall.EISDIR) {
		t.Errorf("directory: %v", err)
	}
}

func TestSymbolicLink(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha beta\n"), 0o644))
	must(t, os.Symlink("a.txt", filepath.Join(dir, "link")))
	if got, err := FileSize(filepath.Join(dir, "link")); err != nil || got != 11 {
		t.Errorf("link: %d, %v", got, err)
	}
}

func TestSparseFileOf5GiB(t *testing.T) {
	path := filepath.Join(t.TempDir(), "sparse.bin")
	must(t, os.WriteFile(path, nil, 0o644))
	must(t, os.Truncate(path, 5<<30))
	if got, err := FileSize(path); err != nil || got != 5368709120 {
		t.Errorf("sparse: %d, %v", got, err)
	}
}

func TestOpenFile(t *testing.T) {
	path := filepath.Join(t.TempDir(), "app.log")
	f, err := os.Create(path)
	must(t, err)
	defer f.Close()
	_, err = f.WriteString("alpha\n") // an *os.File has no buffer
	must(t, err)
	if got, err := FileSize(path); err != nil || got != 6 {
		t.Errorf("open: %d, %v", got, err)
	}
}
