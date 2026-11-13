// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-symbolic-link/.

package makesymlink

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"testing"
)

func tree(t *testing.T) string {
	t.Helper()
	dir := t.TempDir()
	if err := os.WriteFile(filepath.Join(dir, "a.txt"), []byte("alpha\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.Mkdir(filepath.Join(dir, "sub"), 0o755); err != nil {
		t.Fatal(err)
	}
	if err := os.WriteFile(filepath.Join(dir, "sub", "b.txt"), []byte("beta\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	return dir
}

func TestCases(t *testing.T) {
	cases := []struct{ name, target, link string }{
		{"file-target", "a.txt", "link"},
		{"directory-target", "sub", "link"},
		{"link-in-subdirectory", "b.txt", "sub/link"},
		{"dangling-target", "missing.txt", "link"},
		{"relative-target", "a.txt", "sub/link"},
		{"parent-target", "../a.txt", "sub/link"},
		{"name-with-spaces", "my notes.txt", "my link"},
		{"non-ascii-name", "café 日本.txt", "lien 日本"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			link := filepath.Join(tree(t), c.link)
			got, err := MakeSymlink(c.target, link)
			if err != nil || got != c.target {
				t.Fatalf("got %q, %v; want %q", got, err, c.target)
			}
			fi, err := os.Lstat(link)
			if err != nil || fi.Mode().Type() != fs.ModeSymlink {
				t.Fatalf("not a symbolic link: %v", err)
			}
		})
	}
}

func TestExists(t *testing.T) {
	dir := tree(t)
	for _, rel := range []string{"a.txt", "sub"} {
		if _, err := MakeSymlink("b.txt", filepath.Join(dir, rel)); !errors.Is(err, fs.ErrExist) {
			t.Errorf("%s: got %v, want fs.ErrExist", rel, err)
		}
	}
}

func TestDanglingLinkExists(t *testing.T) {
	link := filepath.Join(tree(t), "link")
	if err := os.Symlink("missing.txt", link); err != nil {
		t.Fatal(err)
	}
	if _, err := MakeSymlink("a.txt", link); !errors.Is(err, fs.ErrExist) {
		t.Fatalf("got %v, want fs.ErrExist", err)
	}
	if got, _ := os.Readlink(link); got != "missing.txt" {
		t.Fatalf("the link changed to %q", got)
	}
}

func TestMissingDirectory(t *testing.T) {
	if _, err := MakeSymlink("a.txt", filepath.Join(tree(t), "nodir", "link")); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("got %v, want fs.ErrNotExist", err)
	}
}

func TestLinkReadsTarget(t *testing.T) {
	dir := tree(t)
	if _, err := MakeSymlink("../a.txt", filepath.Join(dir, "sub", "up")); err != nil {
		t.Fatal(err)
	}
	data, err := os.ReadFile(filepath.Join(dir, "sub", "up"))
	if err != nil || string(data) != "alpha\n" {
		t.Fatalf("got %q, %v", data, err)
	}
}

func TestAbsoluteTarget(t *testing.T) {
	dir := tree(t)
	target := filepath.Join(dir, "a.txt")
	if got, err := MakeSymlink(target, filepath.Join(dir, "link")); err != nil || got != target {
		t.Fatalf("got %q, %v", got, err)
	}
}
