// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

package deletefile

import (
	"errors"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"testing"
)

func must(t *testing.T, err error) {
	t.Helper()
	if err != nil {
		t.Fatal(err)
	}
}

func lexists(path string) bool {
	_, err := os.Lstat(path)
	return err == nil
}

func TestCases(t *testing.T) {
	cases := []struct {
		name, path string
		content    *string
		want       bool
	}{
		{"delete", "a.txt", ptr("alpha\n"), true},
		{"missing", "a.txt", nil, false},
		{"empty-file", "a.txt", ptr(""), true},
		{"missing-parent", "no-such-dir/a.txt", nil, false},
		{"name-with-spaces", "my notes.txt", ptr("alpha\n"), true},
		{"non-ascii-name", "café 日本.txt", ptr("alpha\n"), true},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			path := filepath.Join(t.TempDir(), c.path)
			if c.content != nil {
				must(t, os.WriteFile(path, []byte(*c.content), 0o644))
			}
			got, err := DeleteFile(path)
			if err != nil || got != c.want {
				t.Fatalf("DeleteFile = %v, %v; want %v", got, err, c.want)
			}
			if lexists(path) {
				t.Fatal("the file is still there")
			}
		})
	}
}

func ptr(s string) *string { return &s }

func TestThroughAFile(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644))
	got, err := DeleteFile(filepath.Join(dir, "a.txt", "b.txt"))
	if got || err != nil {
		t.Fatalf("got %v, %v", got, err)
	}
}

func TestDirectory(t *testing.T) {
	dir := filepath.Join(t.TempDir(), "dir")
	must(t, os.Mkdir(dir, 0o755))
	if _, err := DeleteFile(dir); !errors.Is(err, ErrIsADirectory) {
		t.Fatalf("err = %v", err)
	}
	if !lexists(dir) {
		t.Fatal("the directory is gone")
	}
}

func TestLinks(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644))
	must(t, os.Mkdir(filepath.Join(dir, "dir"), 0o755))
	must(t, os.Symlink("a.txt", filepath.Join(dir, "to-file")))
	must(t, os.Symlink("dir", filepath.Join(dir, "to-dir")))
	must(t, os.Symlink("missing.txt", filepath.Join(dir, "broken")))
	for _, link := range []string{"to-file", "to-dir", "broken"} {
		got, err := DeleteFile(filepath.Join(dir, link))
		if !got || err != nil || lexists(filepath.Join(dir, link)) {
			t.Fatalf("%s: got %v, %v", link, got, err)
		}
	}
	if b, _ := os.ReadFile(filepath.Join(dir, "a.txt")); string(b) != "alpha\n" {
		t.Fatal("the target is changed")
	}
	if !lexists(filepath.Join(dir, "dir")) {
		t.Fatal("the target directory is gone")
	}
}

func TestReadOnlyFile(t *testing.T) {
	path := filepath.Join(t.TempDir(), "a.txt")
	must(t, os.WriteFile(path, []byte("alpha\n"), 0o444))
	if got, err := DeleteFile(path); !got || err != nil || lexists(path) {
		t.Fatalf("got %v, %v", got, err)
	}
}

func TestDirectoryWithNoWritePermission(t *testing.T) {
	if os.Geteuid() == 0 {
		t.Skip("root can write to each directory")
	}
	locked := filepath.Join(t.TempDir(), "locked")
	must(t, os.Mkdir(locked, 0o755))
	path := filepath.Join(locked, "a.txt")
	must(t, os.WriteFile(path, []byte("alpha\n"), 0o644))
	must(t, os.Chmod(locked, 0o500))
	defer os.Chmod(locked, 0o700)
	if _, err := DeleteFile(path); !errors.Is(err, fs.ErrPermission) {
		t.Fatalf("err = %v", err)
	}
	if !lexists(path) {
		t.Fatal("the file is gone")
	}
}

func TestOpenFile(t *testing.T) {
	path := filepath.Join(t.TempDir(), "a.txt")
	must(t, os.WriteFile(path, []byte("alpha\n"), 0o644))
	f, err := os.Open(path)
	must(t, err)
	defer f.Close()
	if got, err := DeleteFile(path); !got || err != nil || lexists(path) {
		t.Fatalf("got %v, %v", got, err)
	}
	b, err := io.ReadAll(f) // on Unix the data stays until the close
	if err != nil || string(b) != "alpha\n" {
		t.Fatalf("read %q, %v", b, err)
	}
}
