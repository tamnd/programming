// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-lines-of-a-file/.

// Package countlines counts the lines of a file without decoding it.
package countlines

import (
	"bytes"
	"errors"
	"io"
	"os"
)

// Count returns the number of LF bytes in r, the number that wc -l prints,
// and the number of lines, which also counts a last line that has no LF.
func Count(r io.Reader) (newlines, lines int64, err error) {
	buf := make([]byte, 64*1024)
	last := byte('\n') // an empty input has no line without an LF
	for {
		n, err := r.Read(buf)
		// Process the bytes first: Read can return n > 0 together with an error.
		if n > 0 {
			newlines += int64(bytes.Count(buf[:n], []byte{'\n'}))
			last = buf[n-1]
		}
		if errors.Is(err, io.EOF) {
			break
		}
		if err != nil {
			return 0, 0, err
		}
	}
	lines = newlines
	if last != '\n' {
		lines++
	}
	return newlines, lines, nil
}

// CountFile counts the lines of the file at path. A missing file gives an
// error for which errors.Is(err, fs.ErrNotExist) is true.
func CountFile(path string) (newlines, lines int64, err error) {
	f, err := os.Open(path)
	if err != nil {
		return 0, 0, err
	}
	defer f.Close()
	return Count(f)
}
