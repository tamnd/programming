// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/lock-a-file/.

//go:build unix

package lockfile

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"testing"
)

func data(t *testing.T) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "data.txt")
	if err := os.WriteFile(path, []byte("alpha\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, held, want string
		expect           bool
	}{
		{"no-lock-exclusive", "none", "exclusive", true},
		{"no-lock-shared", "none", "shared", true},
		{"exclusive-with-exclusive", "exclusive", "exclusive", false},
		{"shared-with-shared", "shared", "shared", true},
		{"shared-with-exclusive", "shared", "exclusive", false},
		{"exclusive-with-shared", "exclusive", "shared", false},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := data(t)
			got, err := TryLock(path, c.held, c.want)
			if err != nil || got != c.expect {
				t.Fatalf("got %v, %v; want %v", got, err, c.expect)
			}
			if b, _ := os.ReadFile(path); string(b) != "alpha\n" {
				t.Fatalf("the file holds %q", b)
			}
		})
	}
}

func TestReleaseFreesTheLock(t *testing.T) {
	path := data(t)
	l, err := TryLockFile(path, "exclusive")
	if err != nil || l == nil {
		t.Fatal("no first lock", err)
	}
	if l2, err := TryLockFile(path, "exclusive"); err != nil || l2 != nil {
		t.Fatal("a second exclusive lock", err)
	}
	if err := l.Release(); err != nil {
		t.Fatal(err)
	}
	l3, err := TryLockFile(path, "exclusive")
	if err != nil || l3 == nil {
		t.Fatal("no lock after the release", err)
	}
	l3.Release()
}

func TestMissingFile(t *testing.T) {
	dir := t.TempDir()
	_, err := TryLockFile(filepath.Join(dir, "missing.txt"), "exclusive")
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("got %v, want not exist", err)
	}
	if entries, _ := os.ReadDir(dir); len(entries) != 0 {
		t.Fatal("the call made a file")
	}
}
