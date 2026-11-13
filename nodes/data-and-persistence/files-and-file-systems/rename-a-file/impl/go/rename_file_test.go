// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rename-a-file/.

package renamefile

import (
	"errors"
	"fmt"
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

func read(t *testing.T, path string) string {
	t.Helper()
	b, err := os.ReadFile(path)
	must(t, err)
	return string(b)
}

func lexists(path string) bool {
	_, err := os.Lstat(path)
	return err == nil
}

func ptr(s string) *string { return &s }

func TestCases(t *testing.T) {
	cases := []struct {
		name, src, dst, srcText string
		dstText                 *string
		want                    string
	}{
		{"rename", "a.txt", "b.txt", "alpha\n", nil, "alpha\n"},
		{"replace-existing", "a.txt", "b.txt", "alpha\n", ptr("beta\n"), "alpha\n"},
		{"same-path", "a.txt", "a.txt", "alpha\n", ptr("alpha\n"), "alpha\n"},
		{"empty-file", "a.txt", "b.txt", "", ptr("beta\n"), ""},
		{"other-directory", "a.txt", "sub/a.txt", "alpha\n", nil, "alpha\n"},
		{"name-with-spaces", "my notes.txt", "old notes.txt", "alpha\n", nil, "alpha\n"},
		{"non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", nil, "alpha\n"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			src, dst := filepath.Join(dir, c.src), filepath.Join(dir, c.dst)
			must(t, os.MkdirAll(filepath.Dir(dst), 0o755))
			if c.dstText != nil {
				must(t, os.WriteFile(dst, []byte(*c.dstText), 0o644))
			}
			must(t, os.WriteFile(src, []byte(c.srcText), 0o644))
			must(t, RenameFile(src, dst))
			if got := read(t, dst); got != c.want {
				t.Fatalf("dst = %q, want %q", got, c.want)
			}
			if src != dst && lexists(src) {
				t.Fatal("src is still there")
			}
		})
	}
}

func TestMissingSrc(t *testing.T) {
	dir := t.TempDir()
	if err := RenameFile(filepath.Join(dir, "a.txt"), filepath.Join(dir, "b.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v", err)
	}
}

func TestMissingDstDir(t *testing.T) {
	dir := t.TempDir()
	src := filepath.Join(dir, "a.txt")
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	if err := RenameFile(src, filepath.Join(dir, "no-such-dir", "b.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v", err)
	}
	if read(t, src) != "alpha\n" {
		t.Fatal("src is changed")
	}
}

func TestDstIsADirectory(t *testing.T) {
	dir := t.TempDir()
	src, sub := filepath.Join(dir, "a.txt"), filepath.Join(dir, "dir")
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	must(t, os.Mkdir(sub, 0o755))
	if err := RenameFile(src, sub); err == nil {
		t.Fatal("no error")
	}
	if info, err := os.Lstat(sub); err != nil || !info.IsDir() || read(t, src) != "alpha\n" {
		t.Fatal("a file is changed")
	}
}

func TestLink(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644))
	must(t, os.Symlink("a.txt", filepath.Join(dir, "link")))
	must(t, RenameFile(filepath.Join(dir, "link"), filepath.Join(dir, "new-link")))
	if target, err := os.Readlink(filepath.Join(dir, "new-link")); err != nil || target != "a.txt" {
		t.Fatalf("target = %q, %v", target, err)
	}
	if lexists(filepath.Join(dir, "link")) {
		t.Fatal("the old link is still there")
	}
}

func TestOpenDstKeepsOldData(t *testing.T) {
	dir := t.TempDir()
	src, dst := filepath.Join(dir, "a.txt"), filepath.Join(dir, "b.txt")
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	must(t, os.WriteFile(dst, []byte("beta\n"), 0o644))
	old, err := os.Open(dst)
	must(t, err)
	defer old.Close()
	must(t, RenameFile(src, dst))
	if b, _ := io.ReadAll(old); string(b) != "beta\n" {
		t.Fatalf("old = %q", b)
	}
	if read(t, dst) != "alpha\n" {
		t.Fatal("dst is not the new file")
	}
}

func TestCrossDevice(t *testing.T) {
	dir := t.TempDir()
	shm, err1 := os.Stat("/dev/shm")
	tmp, err2 := os.Stat(dir)
	if err1 != nil || err2 != nil || shm.Sys().(*syscall.Stat_t).Dev == tmp.Sys().(*syscall.Stat_t).Dev {
		t.Skip("no other file system at /dev/shm")
	}
	src := filepath.Join(dir, "a.txt")
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	dst := fmt.Sprintf("/dev/shm/rename-file-%d.txt", os.Getpid())
	if err := RenameFile(src, dst); !errors.Is(err, syscall.EXDEV) {
		t.Fatalf("err = %v", err)
	}
	if read(t, src) != "alpha\n" || lexists(dst) {
		t.Fatal("a file is changed")
	}
}
