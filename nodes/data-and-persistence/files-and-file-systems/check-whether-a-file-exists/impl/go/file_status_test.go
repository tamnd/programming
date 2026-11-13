// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

package filestatus

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
)

func tree(t *testing.T) string {
	t.Helper()
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644))
	must(t, os.WriteFile(filepath.Join(dir, "empty.txt"), nil, 0o644))
	must(t, os.Mkdir(filepath.Join(dir, "dir"), 0o755))
	return dir
}

func must(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
}

func check(t *testing.T, path, want string) {
	t.Helper()
	got, err := FileStatus(path)
	if err != nil || got != want {
		t.Fatalf("FileStatus(%q) = %q, %v; want %q", path, got, err, want)
	}
}

func TestStatus(t *testing.T) {
	dir := tree(t)
	for name, want := range map[string]string{
		"a.txt":             "file",
		"empty.txt":         "file",
		"dir":               "directory",
		"dir/":              "directory",
		"missing.txt":       "missing",
		"no-such-dir/a.txt": "missing",
		"a.txt/b.txt":       "missing",
	} {
		check(t, dir+"/"+name, want) // filepath.Join drops a trailing slash
	}
}

func TestLinks(t *testing.T) {
	dir := tree(t)
	must(t, os.Symlink("a.txt", filepath.Join(dir, "to-file")))
	must(t, os.Symlink("dir", filepath.Join(dir, "to-dir")))
	must(t, os.Symlink("missing.txt", filepath.Join(dir, "to-nothing")))
	check(t, filepath.Join(dir, "to-file"), "file")
	check(t, filepath.Join(dir, "to-dir"), "directory")
	check(t, filepath.Join(dir, "to-nothing"), "broken-link")
}

func TestDevice(t *testing.T) {
	if _, err := os.Lstat("/dev/null"); err != nil {
		t.Skip("no /dev/null")
	}
	check(t, "/dev/null", "other")
}

func TestNamedPipe(t *testing.T) {
	pipe := filepath.Join(tree(t), "pipe")
	must(t, syscall.Mkfifo(pipe, 0o644))
	check(t, pipe, "other")
}

func TestPermissionError(t *testing.T) {
	if os.Geteuid() == 0 {
		t.Skip("root can search each directory")
	}
	locked := filepath.Join(tree(t), "locked")
	must(t, os.Mkdir(locked, 0o755))
	must(t, os.WriteFile(filepath.Join(locked, "a.txt"), []byte("alpha\n"), 0o644))
	must(t, os.Chmod(locked, 0))
	defer os.Chmod(locked, 0o700)
	if _, err := FileStatus(filepath.Join(locked, "a.txt")); !errors.Is(err, fs.ErrPermission) {
		t.Fatalf("err = %v, want fs.ErrPermission", err)
	}
}
