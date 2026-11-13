// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

package setmode

import (
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
)

func newFile(t *testing.T, name string) string {
	t.Helper()
	p := filepath.Join(t.TempDir(), name)
	if err := os.WriteFile(p, []byte("alpha\n"), 0o644); err != nil {
		t.Fatal(err)
	}
	if err := os.Chmod(p, 0o644); err != nil {
		t.Fatal(err)
	}
	return p
}

func unixMode(t *testing.T, p string) uint32 {
	t.Helper()
	var st syscall.Stat_t
	if err := syscall.Stat(p, &st); err != nil {
		t.Fatal(err)
	}
	return uint32(st.Mode) & 0o7777
}

func TestCases(t *testing.T) {
	for _, c := range []struct {
		name, rel, mode string
		result          uint32
	}{
		{"owner-read-write-group-read", "a.txt", "640", 0o640},
		{"owner-only", "a.txt", "600", 0o600},
		{"executable", "a.txt", "755", 0o755},
		{"same-mode", "a.txt", "644", 0o644},
		{"no-permissions", "a.txt", "000", 0},
		{"leading-zero", "a.txt", "0640", 0o640},
		{"set-user-id", "a.txt", "4755", 0o4755},
		{"name-with-spaces", "my notes.txt", "640", 0o640},
		{"non-ascii-name", "café 日本.txt", "640", 0o640},
	} {
		t.Run(c.name, func(t *testing.T) {
			p := newFile(t, c.rel)
			got, err := SetMode(p, c.mode)
			if err != nil || got != c.result {
				t.Fatalf("SetMode(%q) = %o, %v; want %o", c.mode, got, err, c.result)
			}
			if m := unixMode(t, p); m != c.result {
				t.Fatalf("stat mode = %o, want %o", m, c.result)
			}
		})
	}
}

func TestInvalidMode(t *testing.T) {
	p := newFile(t, "a.txt")
	for _, mode := range []string{"648", "64", "10640", "u+x", "", "640\n"} {
		if _, err := SetMode(p, mode); !errors.Is(err, ErrInvalidMode) {
			t.Errorf("SetMode(%q) error = %v, want ErrInvalidMode", mode, err)
		}
	}
	if m := unixMode(t, p); m != 0o644 {
		t.Fatalf("mode = %o, want 644", m)
	}
}

func TestMissing(t *testing.T) {
	_, err := SetMode(filepath.Join(t.TempDir(), "missing.txt"), "640")
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("error = %v, want fs.ErrNotExist", err)
	}
}

func TestSymbolicLinkChangesTarget(t *testing.T) {
	p := newFile(t, "a.txt")
	link := filepath.Join(filepath.Dir(p), "link")
	if err := os.Symlink("a.txt", link); err != nil {
		t.Fatal(err)
	}
	if got, err := SetMode(link, "600"); err != nil || got != 0o600 {
		t.Fatalf("SetMode(link) = %o, %v", got, err)
	}
	if m := unixMode(t, p); m != 0o600 {
		t.Fatalf("target mode = %o, want 600", m)
	}
}

func TestDirectory(t *testing.T) {
	d := filepath.Join(t.TempDir(), "dir")
	if err := os.Mkdir(d, 0o755); err != nil {
		t.Fatal(err)
	}
	if got, err := SetMode(d, "700"); err != nil || got != 0o700 {
		t.Fatalf("SetMode(dir) = %o, %v", got, err)
	}
}

func TestUmaskDoesNotApply(t *testing.T) {
	p := newFile(t, "a.txt")
	old := syscall.Umask(0o077)
	defer syscall.Umask(old)
	if got, err := SetMode(p, "666"); err != nil || got != 0o666 {
		t.Fatalf("SetMode = %o, %v; want 666", got, err)
	}
}
