// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-binary-file/.

// Package readbytes reads the bytes of a file with no text decoding.
package readbytes

import (
	"errors"
	"io"
	"os"
)

// ReadBytes returns all of the bytes of the file at path.
func ReadBytes(path string) ([]byte, error) {
	return os.ReadFile(path)
}

// ReadExact returns the first n bytes of the file at path. If the file has
// fewer than n bytes, the error is io.ErrUnexpectedEOF.
func ReadExact(path string, n int) ([]byte, error) {
	f, err := os.Open(path)
	if err != nil {
		return nil, err
	}
	defer f.Close()
	data := make([]byte, n)
	// io.ReadFull calls Read again until data is full. It gives io.EOF when
	// it reads no byte at all, so treat that as a short file too.
	if _, err := io.ReadFull(f, data); err != nil {
		if errors.Is(err, io.EOF) {
			err = io.ErrUnexpectedEOF
		}
		return nil, err
	}
	return data, nil
}
