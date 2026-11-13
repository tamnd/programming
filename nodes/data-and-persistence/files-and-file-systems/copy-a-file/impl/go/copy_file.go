// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/copy-a-file/.

// Package copyfile copies a file.
package copyfile

import (
	"errors"
	"io"
	"io/fs"
	"os"
	"syscall"
)

// ErrSameFile is the error when src and dst name the same file.
var ErrSameFile = errors.New("src and dst are the same file")

// CopyFile copies the bytes and the permission bits of the file at src to
// dst, and replaces a file at dst. Go has no function that copies a file, so
// the code opens both files and calls io.Copy. On Linux, io.Copy to an
// *os.File uses copy_file_range or sendfile. The code checks first that dst
// is not a directory and not the same file as src, because O_TRUNC on the
// same file makes it empty. The error of Close is returned, because a write
// error can come only at the close.
func CopyFile(src, dst string) (err error) {
	in, err := os.Open(src)
	if err != nil {
		return err
	}
	defer in.Close()
	s, err := in.Stat()
	if err != nil {
		return err
	}
	if s.IsDir() {
		return &fs.PathError{Op: "copy", Path: src, Err: syscall.EISDIR}
	}
	if d, err := os.Stat(dst); err == nil {
		if d.IsDir() {
			return &fs.PathError{Op: "copy", Path: dst, Err: syscall.EISDIR}
		}
		if os.SameFile(s, d) {
			return &fs.PathError{Op: "copy", Path: dst, Err: ErrSameFile}
		}
	}
	out, err := os.OpenFile(dst, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, s.Mode().Perm())
	if err != nil {
		return err
	}
	defer func() {
		if cerr := out.Close(); err == nil {
			err = cerr
		}
	}()
	if _, err = io.Copy(out, in); err != nil {
		return err
	}
	// The mode of OpenFile is used only for a new file, and the umask
	// changes it, so the code sets the bits again.
	return out.Chmod(s.Mode().Perm())
}
