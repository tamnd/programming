// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-temporary-file/.

// Package tempfile makes a temporary file with os.CreateTemp.
package tempfile

import (
	"errors"
	"os"
	"strings"
)

// ErrInvalidPrefix is the error for a prefix with a slash.
var ErrInvalidPrefix = errors.New("invalid prefix")

// MakeTempFile makes a new, empty file in dir whose name starts with prefix,
// and returns its path. os.CreateTemp makes the file with O_EXCL and the mode
// 0o600. In its pattern, the random string replaces the last "*", so a "*" is
// put after prefix: then a "*" in prefix stays as it is.
func MakeTempFile(dir, prefix string) (string, error) {
	if strings.ContainsRune(prefix, '/') {
		return "", ErrInvalidPrefix
	}
	f, err := os.CreateTemp(dir, prefix+"*")
	if err != nil {
		return "", err
	}
	name := f.Name()
	if err := f.Close(); err != nil {
		os.Remove(name)
		return "", err
	}
	return name, nil
}

// UseTempFile writes data to a new temporary file, reads it back, removes the
// file, and returns the text.
func UseTempFile(dir, prefix, data string) (string, error) {
	name, err := MakeTempFile(dir, prefix)
	if err != nil {
		return "", err
	}
	defer os.Remove(name)
	if err := os.WriteFile(name, []byte(data), 0o600); err != nil {
		return "", err
	}
	b, err := os.ReadFile(name)
	if err != nil {
		return "", err
	}
	return string(b), nil
}
