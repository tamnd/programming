// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

package tempfile

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"strings"
	"testing"
)

func newDir(t *testing.T) string {
	t.Helper()
	dir := t.TempDir()
	if err := os.Mkdir(filepath.Join(dir, "sub"), 0o755); err != nil {
		t.Fatal(err)
	}
	return dir
}

func names(t *testing.T, dir string) []string {
	t.Helper()
	entries, err := os.ReadDir(dir)
	if err != nil {
		t.Fatal(err)
	}
	var out []string
	for _, e := range entries {
		out = append(out, e.Name())
	}
	return out
}

func TestCases(t *testing.T) {
	cases := []struct{ name, rel, prefix, data string }{
		{"one-line", ".", "tmp-", "alpha\n"},
		{"many-lines", ".", "report-", "alpha\nbeta\ngamma\n"},
		{"in-subdirectory", "sub", "tmp-", "alpha\n"},
		{"empty-prefix", ".", "", "alpha\n"},
		{"empty-data", ".", "tmp-", ""},
		{"non-ascii-data", ".", "tmp-", "café 日本\n"},
		{"prefix-with-spaces", ".", "my temp ", "alpha\n"},
		{"non-ascii-prefix", ".", "café-", "alpha\n"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			dir := newDir(t)
			got, err := UseTempFile(filepath.Join(dir, c.rel), c.prefix, c.data)
			if err != nil || got != c.data {
				t.Fatalf("got %q, %v; want %q", got, err, c.data)
			}
			if n := names(t, dir); len(n) != 1 || n[0] != "sub" {
				t.Fatalf("the directory holds %q", n)
			}
			if n := names(t, filepath.Join(dir, "sub")); len(n) != 0 {
				t.Fatalf("sub holds %q", n)
			}
		})
	}
}

func TestMakeTempFile(t *testing.T) {
	for _, prefix := range []string{"tmp-", "", "my temp ", "café-", "a*b-"} {
		dir := newDir(t)
		path, err := MakeTempFile(dir, prefix)
		if err != nil {
			t.Fatal(err)
		}
		name := filepath.Base(path)
		if filepath.Dir(path) != dir || !strings.HasPrefix(name, prefix) || len(name) < len(prefix)+6 {
			t.Fatalf("prefix %q: the path is %q", prefix, path)
		}
		fi, err := os.Lstat(path)
		if err != nil {
			t.Fatal(err)
		}
		if !fi.Mode().IsRegular() || fi.Mode().Perm() != 0o600 || fi.Size() != 0 {
			t.Fatalf("prefix %q: mode %v, size %d", prefix, fi.Mode(), fi.Size())
		}
	}
}

func TestTwoCallsGiveTwoFiles(t *testing.T) {
	dir := newDir(t)
	a, err1 := MakeTempFile(dir, "tmp-")
	b, err2 := MakeTempFile(dir, "tmp-")
	if err1 != nil || err2 != nil || a == b || len(names(t, dir)) != 3 {
		t.Fatalf("%q %v, %q %v", a, err1, b, err2)
	}
}

func TestErrors(t *testing.T) {
	dir := newDir(t)
	if _, err := UseTempFile(filepath.Join(dir, "nodir"), "tmp-", "alpha\n"); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("missing directory: %v", err)
	}
	if _, err := UseTempFile(dir, "sub/tmp-", "alpha\n"); !errors.Is(err, ErrInvalidPrefix) {
		t.Fatalf("prefix with a slash: %v", err)
	}
	if n := names(t, filepath.Join(dir, "sub")); len(n) != 0 {
		t.Fatalf("sub holds %q", n)
	}
}
