// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

package truncatefile

import (
	"errors"
	"io"
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

func check(t *testing.T, path, want string) {
	t.Helper()
	got, err := os.ReadFile(path)
	must(t, err)
	if string(got) != want {
		t.Errorf("got %q, want %q", got, want)
	}
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, path    string
		length        int64
		before, after string
	}{
		{"shrink", "a.txt", 5, "alpha beta\n", "alpha"},
		{"extend", "a.txt", 8, "alpha", "alpha\x00\x00\x00"},
		{"same-length", "a.txt", 6, "alpha\n", "alpha\n"},
		{"to-zero", "a.txt", 0, "alpha\n", ""},
		{"empty-file", "a.txt", 3, "", "\x00\x00\x00"},
		{"name-with-spaces", "my notes.txt", 2, "alpha\n", "al"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), c.path)
			must(t, os.WriteFile(path, []byte(c.before), 0o644))
			must(t, TruncateFile(path, c.length))
			check(t, path, c.after)
		})
	}
}

func TestErrors(t *testing.T) {
	dir := t.TempDir()
	path := filepath.Join(dir, "a.txt")
	if err := TruncateFile(path, 0); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("missing: got %v", err)
	}
	if _, err := os.Lstat(path); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("the call made a file: %v", err)
	}
	must(t, os.Mkdir(filepath.Join(dir, "dir"), 0o755))
	if err := TruncateFile(filepath.Join(dir, "dir"), 0); !errors.Is(err, syscall.EISDIR) {
		t.Errorf("directory: got %v", err)
	}
	must(t, os.WriteFile(path, []byte("alpha\n"), 0o644))
	if err := TruncateFile(path, -1); !errors.Is(err, syscall.EINVAL) {
		t.Errorf("negative: got %v", err)
	}
	check(t, path, "alpha\n")
}

func TestOpenInAppendMode(t *testing.T) {
	path := filepath.Join(t.TempDir(), "app.log")
	must(t, os.WriteFile(path, []byte("alpha beta\n"), 0o644))
	log, err := os.OpenFile(path, os.O_WRONLY|os.O_APPEND, 0)
	must(t, err)
	must(t, TruncateFile(path, 0))
	_, err = log.WriteString("new\n")
	must(t, err)
	must(t, log.Close())
	check(t, path, "new\n")
}

func TestOpenAfterTheNewEnd(t *testing.T) {
	path := filepath.Join(t.TempDir(), "data.bin")
	must(t, os.WriteFile(path, []byte("alpha beta\n"), 0o644))
	f, err := os.OpenFile(path, os.O_WRONLY, 0)
	must(t, err)
	_, err = f.Seek(0, io.SeekEnd)
	must(t, err)
	must(t, TruncateFile(path, 5))
	_, err = f.WriteString("X")
	must(t, err)
	must(t, f.Close())
	check(t, path, "alpha\x00\x00\x00\x00\x00\x00X")
}
