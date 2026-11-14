// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-lines-from-a-file/.

// Package removelines removes lines from a file and writes the rest back under
// the same name.
package removelines

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
)

// ErrBadRange is the error when start is less than 1 or count is less than 0.
var ErrBadRange = errors.New("removelines: start must be 1 or more and count 0 or more")

// RemoveLines removes count lines from the file at path, from line start, and
// returns how many it removed. Line 1 is the first line. A line ends at LF,
// and the LF goes with the line. The result is less than count if the file
// ends first. If no line goes, the file is not written. Otherwise the rest of
// the file goes to a new file in the same directory, and os.Rename puts it in
// the place of the old one. A missing file gives an error for which
// errors.Is(err, fs.ErrNotExist) is true.
func RemoveLines(path string, start, count int) (removed int, err error) {
	if start < 1 || count < 0 {
		return 0, fmt.Errorf("%w: start %d, count %d", ErrBadRange, start, count)
	}
	src, err := os.Open(path)
	if err != nil {
		return 0, err
	}
	defer src.Close()
	info, err := src.Stat()
	if err != nil {
		return 0, err
	}
	tmp, err := os.CreateTemp(filepath.Dir(path), "."+filepath.Base(path)+".*.tmp")
	if err != nil {
		return 0, err
	}
	// On each return before the rename, close and remove the new file.
	renamed := false
	defer func() {
		if !renamed {
			tmp.Close()
			os.Remove(tmp.Name())
		}
	}()
	r := bufio.NewReader(src)
	w := bufio.NewWriter(tmp)
	for n := 1; ; n++ {
		// ReadBytes reads up to and with the next LF, so a CR stays in the
		// line. At the end of the file, it returns the last line with no LF
		// and io.EOF.
		line, err := r.ReadBytes('\n')
		if len(line) > 0 {
			if n >= start && n-start < count {
				removed++
			} else if _, werr := w.Write(line); werr != nil {
				return 0, werr
			}
		}
		if errors.Is(err, io.EOF) {
			break
		}
		if err != nil {
			return 0, err
		}
	}
	if removed == 0 {
		return 0, nil
	}
	if err := w.Flush(); err != nil {
		return 0, err
	}
	// CreateTemp makes the file with mode 0600.
	if err := tmp.Chmod(info.Mode().Perm()); err != nil {
		return 0, err
	}
	if err := tmp.Sync(); err != nil {
		return 0, err
	}
	if err := tmp.Close(); err != nil {
		return 0, err
	}
	if err := os.Rename(tmp.Name(), path); err != nil {
		return 0, err
	}
	renamed = true
	return removed, nil
}
