// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-size-of-a-file/.

// Package filesize gets the size of a file.
package filesize

import (
	"io/fs"
	"os"
	"syscall"
)

// FileSize returns the size in bytes of the regular file at path. os.Stat
// follows a symbolic link, so a link gives the size of its target. A missing
// file gives a *fs.PathError with fs.ErrNotExist. os.Stat also gives a size
// for a directory, but it is not the size of a file, so the code returns a
// *fs.PathError with syscall.EISDIR.
func FileSize(path string) (int64, error) {
	fi, err := os.Stat(path)
	if err != nil {
		return 0, err
	}
	if fi.IsDir() {
		return 0, &fs.PathError{Op: "size", Path: path, Err: syscall.EISDIR}
	}
	return fi.Size(), nil
}
