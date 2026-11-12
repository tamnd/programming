// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-whole-file/.

// Package readtext reads all of a UTF-8 text file in one call.
package readtext

import (
	"errors"
	"fmt"
	"os"
	"unicode/utf8"
)

// ErrInvalidUTF8 reports that the bytes of the file are not valid UTF-8.
var ErrInvalidUTF8 = errors.New("invalid UTF-8")

// ReadText returns all of the text of a UTF-8 file, with no change to it.
// os.ReadFile does not check the bytes, so ReadText checks them.
func ReadText(path string) (string, error) {
	data, err := os.ReadFile(path)
	if err != nil {
		return "", err
	}
	if !utf8.Valid(data) {
		return "", fmt.Errorf("%s: %w", path, ErrInvalidUTF8)
	}
	return string(data), nil
}
