// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-and-write-with-buffers/.

// Package copybuffered copies a file through a buffered reader and a
// buffered writer.
package copybuffered

import (
	"bufio"
	"errors"
	"io"
	"os"
)

// ErrInvalidSize is the error for a buffer size below 1.
var ErrInvalidSize = errors.New("copybuffered: size must be at least 1")

// CopyBuffered copies src to dst through buffers of size bytes and returns
// the number of bytes copied. bufio uses at least 16 bytes for a Reader.
//
// The bytes that are still in the Writer go to the file only at Flush, so
// the error of Flush is the error of the last write. The error of Close is
// returned too.
func CopyBuffered(src, dst string, size int) (n int64, err error) {
	if size < 1 {
		return 0, ErrInvalidSize
	}
	in, err := os.Open(src)
	if err != nil {
		return 0, err
	}
	defer in.Close()
	out, err := os.Create(dst)
	if err != nil {
		return 0, err
	}
	defer func() {
		if cerr := out.Close(); err == nil {
			err = cerr
		}
	}()
	r := bufio.NewReaderSize(in, size)
	w := bufio.NewWriterSize(out, size)
	chunk := make([]byte, size)
	for {
		m, rerr := r.Read(chunk)
		if m > 0 {
			if _, werr := w.Write(chunk[:m]); werr != nil {
				return n, werr
			}
			n += int64(m)
		}
		if rerr == io.EOF {
			break
		}
		if rerr != nil {
			return n, rerr
		}
	}
	return n, w.Flush()
}
