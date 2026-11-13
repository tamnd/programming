// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/set-file-permissions/.

// Package setmode sets the permissions of a file.
package setmode

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"regexp"
	"strconv"
)

// ErrInvalidMode is the error for a mode that is not three or four octal digits.
var ErrInvalidMode = errors.New("invalid mode")

// Without the m flag, $ matches only at the end of the text, so "640\n" is not valid.
var modeRE = regexp.MustCompile(`^[0-7]{3,4}$`)

// The Unix bits for setuid, setgid and sticky, and the fs.FileMode bits of Go.
var special = []struct {
	unix uint32
	mode fs.FileMode
}{{0o4000, fs.ModeSetuid}, {0o2000, fs.ModeSetgid}, {0o1000, fs.ModeSticky}}

// SetMode sets the mode of the file at path, and returns the new permission
// bits in the Unix layout (st_mode & 07777). mode is three or four octal
// digits, as for the chmod command. Go keeps setuid, setgid and sticky in its
// own bits, so the code maps them in both directions. os.Chmod follows a
// symbolic link, and the umask does not apply. A missing file gives a
// *fs.PathError with fs.ErrNotExist.
func SetMode(path, mode string) (uint32, error) {
	if !modeRE.MatchString(mode) {
		return 0, fmt.Errorf("set mode %q: %w", mode, ErrInvalidMode)
	}
	n, _ := strconv.ParseUint(mode, 8, 32)
	m := fs.FileMode(n) & fs.ModePerm
	for _, s := range special {
		if uint32(n)&s.unix != 0 {
			m |= s.mode
		}
	}
	if err := os.Chmod(path, m); err != nil {
		return 0, err
	}
	fi, err := os.Stat(path)
	if err != nil {
		return 0, err
	}
	result := uint32(fi.Mode().Perm())
	for _, s := range special {
		if fi.Mode()&s.mode != 0 {
			result |= s.unix
		}
	}
	return result, nil
}
