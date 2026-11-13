// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-modification-time-of-a-file/.

// Package filemtime gets the modification time of a file.
package filemtime

import "os"

// FileMtime returns the modification time of the file at path, in whole
// seconds since 1970-01-01T00:00:00Z. os.Stat follows a symbolic link.
// time.Time.Unix gives the floor, also before 1970. A missing file gives a
// *fs.PathError with fs.ErrNotExist.
func FileMtime(path string) (int64, error) {
	fi, err := os.Stat(path)
	if err != nil {
		return 0, err
	}
	return fi.ModTime().Unix(), nil
}
