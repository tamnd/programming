// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

package copybuffered

import (
	"bytes"
	"errors"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
)

func pattern(n int) []byte {
	b := make([]byte, n)
	for i := range b {
		b[i] = byte(i)
	}
	return b
}

var text = []byte("alpha\nbeta\ngamma\n")

var cases = []struct {
	name string
	data []byte
	size int
}{
	{"text", text, 4096},
	{"several-buffers", text, 4},
	{"empty", []byte{}, 8},
	{"smaller-than-buffer", []byte("12345"), 8},
	{"exact-buffer", []byte("12345678"), 8},
	{"one-past-buffer", []byte("123456789"), 8},
	{"size-one", text, 1},
	{"binary", pattern(256), 64},
	{"large", pattern(1 << 20), 4096},
	{"large-odd-buffer", pattern(1 << 20), 4093},
}

func write(t *testing.T, name string, data []byte) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), name)
	if err := os.WriteFile(path, data, 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestCopy(t *testing.T) {
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			src := write(t, "in.bin", c.data)
			dst := filepath.Join(filepath.Dir(src), "out.bin")
			n, err := CopyBuffered(src, dst, c.size)
			if err != nil || n != int64(len(c.data)) {
				t.Fatalf("CopyBuffered = %d, %v; want %d, nil", n, err, len(c.data))
			}
			got, err := os.ReadFile(dst)
			if err != nil || !bytes.Equal(got, c.data) {
				t.Fatalf("dst has %d bytes that are not the same as src (%v)", len(got), err)
			}
		})
	}
}

func TestReplacesALongerFile(t *testing.T) {
	src := write(t, "in.bin", []byte("new"))
	dst := filepath.Join(filepath.Dir(src), "out.bin")
	if err := os.WriteFile(dst, []byte("old and longer"), 0o644); err != nil {
		t.Fatal(err)
	}
	if n, err := CopyBuffered(src, dst, 8); n != 3 || err != nil {
		t.Fatalf("CopyBuffered = %d, %v", n, err)
	}
	if got, _ := os.ReadFile(dst); string(got) != "new" {
		t.Fatalf("dst = %q", got)
	}
}

func TestMissingSrc(t *testing.T) {
	dir := t.TempDir()
	_, err := CopyBuffered(filepath.Join(dir, "missing.bin"), filepath.Join(dir, "out.bin"), 8)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v, want fs.ErrNotExist", err)
	}
	if _, err := os.Stat(filepath.Join(dir, "out.bin")); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("dst was made")
	}
}

func TestMissingDstDir(t *testing.T) {
	src := write(t, "in.bin", text)
	_, err := CopyBuffered(src, filepath.Join(filepath.Dir(src), "no-such-dir", "out.bin"), 8)
	if !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("err = %v, want fs.ErrNotExist", err)
	}
}

func TestInvalidSize(t *testing.T) {
	src := write(t, "in.bin", text)
	for _, size := range []int{0, -1} {
		if _, err := CopyBuffered(src, filepath.Join(filepath.Dir(src), "out.bin"), size); !errors.Is(err, ErrInvalidSize) {
			t.Fatalf("size %d: err = %v", size, err)
		}
	}
}

func TestFullDevice(t *testing.T) {
	if _, err := os.Stat("/dev/full"); err != nil {
		t.Skip("no /dev/full")
	}
	// 17 bytes stay in the buffer, so the error comes only at Flush.
	for _, n := range []int{17, 1 << 20} {
		src := write(t, "in.bin", pattern(n))
		if _, err := CopyBuffered(src, "/dev/full", 4096); !errors.Is(err, syscall.ENOSPC) {
			t.Fatalf("%d bytes: err = %v, want ENOSPC", n, err)
		}
	}
}
