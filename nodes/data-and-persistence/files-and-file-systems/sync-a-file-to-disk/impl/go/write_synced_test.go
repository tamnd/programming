// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sync-a-file-to-disk/.

package syncfile

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
)

func names(t *testing.T, dir string) []string {
	t.Helper()
	entries, err := os.ReadDir(dir)
	if err != nil {
		t.Fatal(err)
	}
	out := []string{}
	for _, e := range entries {
		out = append(out, e.Name())
	}
	return out
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, text string
		before     *string
	}{
		{"new-file", "alpha\nbeta\n", nil},
		{"replace-longer", "short\n", ptr("a much longer old line\n")},
		{"many-lines", "one\ntwo\nthree\nfour\n", nil},
		{"empty-replace", "", ptr("old\n")},
		{"empty-new", "", nil},
		{"no-final-newline", "last line", nil},
		{"non-ascii", "naïve café\n日本語\n🙂\n", nil},
		{"crlf", "one\r\ntwo\r\n", nil},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			path := filepath.Join(dir, "out.txt")
			if c.before != nil {
				if err := os.WriteFile(path, []byte(*c.before), 0o644); err != nil {
					t.Fatal(err)
				}
			}
			if err := WriteSynced(path, c.text); err != nil {
				t.Fatal(err)
			}
			got, err := os.ReadFile(path)
			if err != nil || string(got) != c.text {
				t.Fatalf("got %q, %v; want %q", got, err, c.text)
			}
			if n := names(t, dir); len(n) != 1 || n[0] != "out.txt" {
				t.Fatalf("the directory holds %v", n)
			}
		})
	}
}

func ptr(s string) *string { return &s }

func TestSyncDirectory(t *testing.T) {
	if err := SyncDirectory(t.TempDir()); err != nil {
		t.Fatal(err)
	}
}

func TestMissingDirectory(t *testing.T) {
	dir := t.TempDir()
	err := WriteSynced(filepath.Join(dir, "missing", "out.txt"), "x\n")
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("got %v, want not exist", err)
	}
	if n := names(t, dir); len(n) != 0 {
		t.Fatalf("the directory holds %v", n)
	}
}

func TestOntoDirectory(t *testing.T) {
	path := filepath.Join(t.TempDir(), "out.txt")
	if err := os.Mkdir(path, 0o755); err != nil {
		t.Fatal(err)
	}
	if err := WriteSynced(path, "x\n"); !errors.Is(err, syscall.EISDIR) {
		t.Fatalf("got %v, want EISDIR", err)
	}
	if fi, err := os.Stat(path); err != nil || !fi.IsDir() {
		t.Fatal("the directory is gone")
	}
}
