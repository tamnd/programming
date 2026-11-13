// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

package copyfile

import (
	"bytes"
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
	// name: src, dst, content of src, content of dst ("-" for no file)
	cases := []struct{ name, src, dst, srcText, dstText string }{
		{"copy", "a.txt", "b.txt", "alpha\n", "-"},
		{"replace-existing", "a.txt", "b.txt", "alpha\n", "beta, a longer text\n"},
		{"empty-file", "a.txt", "b.txt", "", "beta\n"},
		{"other-directory", "a.txt", "sub/a.txt", "alpha\n", "-"},
		{"name-with-spaces", "my notes.txt", "copy of my notes.txt", "alpha\n", "-"},
		{"non-ascii-name", "a.txt", "café 日本.txt", "alpha\n", "-"},
		{"binary-data", "a.bin", "b.bin", "\x00\r\nÿ\x00", "-"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := t.TempDir()
			src, dst := filepath.Join(dir, c.src), filepath.Join(dir, c.dst)
			must(t, os.MkdirAll(filepath.Dir(dst), 0o755))
			if c.dstText != "-" {
				must(t, os.WriteFile(dst, []byte(c.dstText), 0o644))
			}
			must(t, os.WriteFile(src, []byte(c.srcText), 0o644))
			must(t, CopyFile(src, dst))
			for _, p := range []string{dst, src} {
				got, err := os.ReadFile(p)
				must(t, err)
				if string(got) != c.srcText {
					t.Errorf("%s: got %q, want %q", p, got, c.srcText)
				}
			}
		})
	}
}

func TestMissing(t *testing.T) {
	dir := t.TempDir()
	src := filepath.Join(dir, "a.txt")
	if err := CopyFile(src, filepath.Join(dir, "b.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("missing src: got %v", err)
	}
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	if err := CopyFile(src, filepath.Join(dir, "no-such-dir", "b.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("missing dst dir: got %v", err)
	}
}

func TestDirectories(t *testing.T) {
	dir := t.TempDir()
	sub, src := filepath.Join(dir, "dir"), filepath.Join(dir, "a.txt")
	must(t, os.Mkdir(sub, 0o755))
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	if err := CopyFile(sub, filepath.Join(dir, "b.txt")); !errors.Is(err, syscall.EISDIR) {
		t.Errorf("src dir: got %v", err)
	}
	if err := CopyFile(src, sub); !errors.Is(err, syscall.EISDIR) {
		t.Errorf("dst dir: got %v", err)
	}
	if entries, _ := os.ReadDir(sub); len(entries) != 0 {
		t.Errorf("dir has %d entries", len(entries))
	}
	if _, err := os.Lstat(filepath.Join(dir, "b.txt")); !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("b.txt: got %v", err)
	}
}

func TestSameFile(t *testing.T) {
	dir := t.TempDir()
	src, link := filepath.Join(dir, "a.txt"), filepath.Join(dir, "b.txt")
	must(t, os.WriteFile(src, []byte("alpha\n"), 0o644))
	must(t, os.Link(src, link))
	for _, dst := range []string{src, link} {
		if err := CopyFile(src, dst); !errors.Is(err, ErrSameFile) {
			t.Errorf("%s: got %v", dst, err)
		}
		if got, _ := os.ReadFile(src); string(got) != "alpha\n" {
			t.Errorf("%s: src is now %q", dst, got)
		}
	}
}

func TestMode(t *testing.T) {
	dir := t.TempDir()
	src, dst, other := filepath.Join(dir, "a.sh"), filepath.Join(dir, "b.sh"), filepath.Join(dir, "c.sh")
	must(t, os.WriteFile(src, []byte("echo alpha\n"), 0o644))
	must(t, os.Chmod(src, 0o750))
	must(t, os.WriteFile(other, []byte("beta\n"), 0o644))
	must(t, os.Chmod(other, 0o640))
	for _, p := range []string{dst, other} {
		must(t, CopyFile(src, p))
		info, err := os.Stat(p)
		must(t, err)
		if info.Mode().Perm() != 0o750 {
			t.Errorf("%s: mode %o", p, info.Mode().Perm())
		}
	}
}

func TestLargeFile(t *testing.T) {
	dir := t.TempDir()
	data := make([]byte, 5_000_003)
	for i := range data {
		data[i] = byte(i * 7 % 251)
	}
	src, dst := filepath.Join(dir, "big.bin"), filepath.Join(dir, "copy.bin")
	must(t, os.WriteFile(src, data, 0o644))
	must(t, CopyFile(src, dst))
	got, err := os.ReadFile(dst)
	must(t, err)
	if !bytes.Equal(got, data) {
		t.Errorf("the copy has %d bytes and is not the same", len(got))
	}
}

func TestLinkAtSrc(t *testing.T) {
	dir := t.TempDir()
	must(t, os.WriteFile(filepath.Join(dir, "target.txt"), []byte("alpha\n"), 0o644))
	must(t, os.Symlink("target.txt", filepath.Join(dir, "link.txt")))
	dst := filepath.Join(dir, "b.txt")
	must(t, CopyFile(filepath.Join(dir, "link.txt"), dst))
	info, err := os.Lstat(dst)
	must(t, err)
	if info.Mode()&fs.ModeSymlink != 0 {
		t.Error("the copy is a link")
	}
	if got, _ := os.ReadFile(dst); string(got) != "alpha\n" {
		t.Errorf("got %q", got)
	}
}
