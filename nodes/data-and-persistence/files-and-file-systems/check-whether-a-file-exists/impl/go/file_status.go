// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-file-exists/.

// Package filestatus tells what is at a path.
package filestatus

import (
	"errors"
	"io/fs"
	"os"
	"syscall"
)

// FileStatus tells what is at path: "file", "directory", "other", "missing"
// or "broken-link". Each error other than "no such entry" is returned.
//
// errors.Is(err, fs.ErrNotExist) is true for ENOENT but not for ENOTDIR,
// which a path through a regular file gives, so the code checks both.
func FileStatus(path string) (string, error) {
	info, err := os.Stat(path) // follows symbolic links
	if err != nil {
		if !errors.Is(err, fs.ErrNotExist) && !errors.Is(err, syscall.ENOTDIR) {
			return "", err
		}
		if l, lerr := os.Lstat(path); lerr == nil && l.Mode()&fs.ModeSymlink != 0 {
			return "broken-link", nil
		}
		return "missing", nil
	}
	switch {
	case info.Mode().IsRegular():
		return "file", nil
	case info.IsDir():
		return "directory", nil
	default:
		return "other", nil
	}
}
