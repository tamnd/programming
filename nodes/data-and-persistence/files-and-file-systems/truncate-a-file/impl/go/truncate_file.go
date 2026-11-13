// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/truncate-a-file/.

// Package truncatefile sets the size of a file.
package truncatefile

import "os"

// TruncateFile sets the size of the existing file at path to length bytes.
// os.Truncate cuts the bytes after length, or adds zero bytes up to length,
// and it does not make a missing file. The error is a *fs.PathError, for
// example with fs.ErrNotExist, syscall.EISDIR, or syscall.EINVAL for a length
// under 0.
func TruncateFile(path string, length int64) error {
	return os.Truncate(path, length)
}
