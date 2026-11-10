// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/list-files-in-a-directory/.

package listentries

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"slices"
	"strings"
	"syscall"
	"testing"
)

// makeTree makes each entry of the tree in a new directory and returns its path.
func makeTree(t *testing.T, tree []string) string {
	t.Helper()
	root := t.TempDir()
	for _, entry := range tree {
		var err error
		if name, target, ok := strings.Cut(entry, " -> "); ok {
			err = os.Symlink(target, filepath.Join(root, name))
		} else if strings.HasSuffix(entry, "/") {
			err = os.Mkdir(filepath.Join(root, entry), 0o755)
		} else {
			err = os.WriteFile(filepath.Join(root, entry), nil, 0o644)
		}
		if err != nil {
			t.Fatal(err)
		}
	}
	return root
}

func TestCases(t *testing.T) {
	tree := []string{"docs/", "docs/guide.md", "docs/img/", "docs/img/logo.png", "readme.md"}
	cases := []struct {
		name      string
		tree      []string
		recursive bool
		want      []string
	}{
		{"flat", []string{"b.txt", "a.txt", "c.txt"}, false, []string{"a.txt", "b.txt", "c.txt"}},
		{"nested", tree, true, tree},
		{"nested-shallow", tree, false, []string{"docs/", "readme.md"}},
		{"empty", nil, true, nil},
		{"empty-subdir", []string{"empty/"}, true, []string{"empty/"}},
		{"hidden", []string{".env", ".git/", ".git/config", "main.go"}, true, []string{".env", ".git/", ".git/config", "main.go"}},
		{"case-order", []string{"a.txt", "B.txt", "c.txt"}, false, []string{"B.txt", "a.txt", "c.txt"}},
		{"dot-before-slash", []string{"a/", "a/x", "a.txt", "a-b"}, true, []string{"a-b", "a.txt", "a/", "a/x"}},
		{"symlink-to-dir", []string{"real/", "real/f", "link -> real"}, true, []string{"link", "real/", "real/f"}},
		{"symlink-to-file", []string{"f", "g -> f"}, true, []string{"f", "g"}},
		{"broken-symlink", []string{"dangling -> nowhere"}, true, []string{"dangling"}},
		{"symlink-loop", []string{"d/", "d/up -> .."}, true, []string{"d/", "d/up"}},
		{"spaces", []string{"my file.txt", "two  spaces/", "two  spaces/x"}, true, []string{"my file.txt", "two  spaces/", "two  spaces/x"}},
		{"non-ascii", []string{"été.txt", "zebra.txt", "apple.txt"}, false, []string{"apple.txt", "zebra.txt", "été.txt"}},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ListEntries(makeTree(t, c.tree), c.recursive)
			if err != nil || !slices.Equal(got, c.want) {
				t.Errorf("ListEntries = %q, %v; want %q, nil", got, err, c.want)
			}
		})
	}
}

func TestMissing(t *testing.T) {
	_, err := ListEntries(filepath.Join(t.TempDir(), "missing"), true)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Errorf("err = %v, want fs.ErrNotExist", err)
	}
}

func TestNotADirectory(t *testing.T) {
	root := makeTree(t, []string{"notes.txt"})
	_, err := ListEntries(filepath.Join(root, "notes.txt"), true)
	if !errors.Is(err, syscall.ENOTDIR) {
		t.Errorf("err = %v, want syscall.ENOTDIR", err)
	}
}
