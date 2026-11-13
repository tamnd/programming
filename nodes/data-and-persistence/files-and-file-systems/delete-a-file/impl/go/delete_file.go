// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/delete-a-file/.

// Package deletefile deletes a file and tells whether a file was there.
package deletefile

import (
	"errors"
	"io/fs"
	"os"
	"syscall"
)

// ErrIsADirectory is the error for a path that names a directory.
var ErrIsADirectory = errors.New("is a directory")

// gone reports whether err means that no entry is at the path: ENOENT, or
// ENOTDIR for a path through a regular file.
func gone(err error) bool {
	return errors.Is(err, fs.ErrNotExist) || errors.Is(err, syscall.ENOTDIR)
}

// DeleteFile deletes the file at path. It gives true if a file was there and
// false if not. os.Remove also deletes an empty directory, so the code looks
// at the entry first, with os.Lstat, and does not delete a directory. A
// symbolic link is deleted, not its target. Each other error goes to the
// caller.
func DeleteFile(path string) (bool, error) {
	info, err := os.Lstat(path)
	if err != nil {
		if gone(err) {
			return false, nil
		}
		return false, err
	}
	if info.IsDir() {
		return false, &fs.PathError{Op: "delete", Path: path, Err: ErrIsADirectory}
	}
	if err := os.Remove(path); err != nil {
		if gone(err) {
			return false, nil // another program deleted it first
		}
		return false, err
	}
	return true, nil
}
