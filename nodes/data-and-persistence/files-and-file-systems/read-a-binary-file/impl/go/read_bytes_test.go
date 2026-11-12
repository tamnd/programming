// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

package readbytes

import (
	"bytes"
	"encoding/hex"
	"errors"
	"io"
	"io/fs"
	"os"
	"path/filepath"
	"syscall"
	"testing"
	"time"
)

func unhex(s string) []byte {
	b, err := hex.DecodeString(s)
	if err != nil {
		panic(err)
	}
	return b
}

func pattern(n int) []byte {
	b := make([]byte, n)
	for i := range b {
		b[i] = byte(i)
	}
	return b
}

var data = map[string][]byte{
	"signature": unhex("89504e470d0a1a0a0000000d49484452"),
	"bytes":     unhex("0001027f80feff"),
	"empty":     {},
	"all-bytes": pattern(256),
	"zeros":     make([]byte, 8),
	"crlf":      []byte("a\r\nb\r\n"),
	"ctrl-z":    unhex("611a62"),
	"not-utf8":  unhex("c328fffe80"),
	"large":     pattern(1 << 20),
}

func write(t *testing.T, name string) string {
	t.Helper()
	path := filepath.Join(t.TempDir(), "input.bin")
	if err := os.WriteFile(path, data[name], 0o644); err != nil {
		t.Fatal(err)
	}
	return path
}

func TestReadBytes(t *testing.T) {
	for name, want := range data {
		t.Run(name, func(t *testing.T) {
			got, err := ReadBytes(write(t, name))
			if err != nil || !bytes.Equal(got, want) {
				t.Fatalf("ReadBytes = %x, %v; want %x", got, err, want)
			}
		})
	}
}

func TestReadExact(t *testing.T) {
	cases := []struct {
		name, data string
		n          int
		want       []byte
	}{
		{"signature-only", "signature", 8, unhex("89504e470d0a1a0a")},
		{"first-four", "all-bytes", 4, unhex("00010203")},
		{"whole-file", "zeros", 8, make([]byte, 8)},
		{"zero-count", "empty", 0, []byte{}},
		{"across-ctrl-z", "ctrl-z", 3, unhex("611a62")},
		{"large", "large", 1 << 20, data["large"]},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			got, err := ReadExact(write(t, c.data), c.n)
			if err != nil || !bytes.Equal(got, c.want) {
				t.Fatalf("ReadExact = %x, %v; want %x", got, err, c.want)
			}
		})
	}
}

func TestShortFile(t *testing.T) {
	cases := []struct {
		name, data string
		n          int
	}{
		{"one-past-end", "signature", 17},
		{"empty-file", "empty", 1},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			_, err := ReadExact(write(t, c.data), c.n)
			if !errors.Is(err, io.ErrUnexpectedEOF) {
				t.Fatalf("err = %v; want io.ErrUnexpectedEOF", err)
			}
		})
	}
}

func TestMissingFile(t *testing.T) {
	path := filepath.Join(t.TempDir(), "missing.bin")
	if _, err := ReadBytes(path); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("ReadBytes err = %v; want fs.ErrNotExist", err)
	}
	if _, err := ReadExact(path, 1); !errors.Is(err, fs.ErrNotExist) {
		t.Fatalf("ReadExact err = %v; want fs.ErrNotExist", err)
	}
}

func TestDirectory(t *testing.T) {
	dir := t.TempDir()
	if _, err := ReadBytes(dir); !errors.Is(err, syscall.EISDIR) {
		t.Fatalf("ReadBytes err = %v; want EISDIR", err)
	}
	if _, err := ReadExact(dir, 1); !errors.Is(err, syscall.EISDIR) {
		t.Fatalf("ReadExact err = %v; want EISDIR", err)
	}
}

// A pipe gives the bytes in two parts, so one Read call gets only the first
// part. ReadExact must read again to get all 8 bytes.
func TestReadsAgain(t *testing.T) {
	fifo := filepath.Join(t.TempDir(), "fifo")
	if err := syscall.Mkfifo(fifo, 0o600); err != nil {
		t.Fatal(err)
	}
	done := make(chan error, 1)
	go func() {
		out, err := os.OpenFile(fifo, os.O_WRONLY, 0)
		if err != nil {
			done <- err
			return
		}
		defer out.Close()
		if _, err := out.Write([]byte("abc")); err != nil {
			done <- err
			return
		}
		time.Sleep(200 * time.Millisecond)
		_, err = out.Write([]byte("defgh"))
		done <- err
	}()
	got, err := ReadExact(fifo, 8)
	if werr := <-done; werr != nil {
		t.Fatal(werr)
	}
	if err != nil || string(got) != "abcdefgh" {
		t.Fatalf("ReadExact = %q, %v; want abcdefgh", got, err)
	}
}
