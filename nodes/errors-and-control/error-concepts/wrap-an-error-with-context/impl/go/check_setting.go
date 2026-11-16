// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

// Package setting reads a port setting and wraps the error of the parser
// with the place of the setting.
package setting

import (
	"errors"
	"fmt"
)

// The errors of ParsePort wrap one of these two values.
var (
	ErrNotANumber = errors.New("not a number")
	ErrOutOfRange = errors.New("out of range")
)

// ParsePort returns the port number in text.
func ParsePort(text string) (int, error) {
	for _, c := range text {
		if c < '0' || c > '9' {
			return 0, fmt.Errorf("%w: '%s'", ErrNotANumber, text)
		}
	}
	if text == "" {
		return 0, fmt.Errorf("%w: '%s'", ErrNotANumber, text)
	}
	port := 0
	for _, c := range text {
		port = port*10 + int(c-'0')
		if port > 65535 {
			return 0, fmt.Errorf("%w: %s", ErrOutOfRange, text)
		}
	}
	if port == 0 {
		return 0, fmt.Errorf("%w: %s", ErrOutOfRange, text)
	}
	return port, nil
}

// ReadPort returns the port number. On an error, it adds where to the
// message, and %w keeps the error of ParsePort for errors.Is and errors.As.
func ReadPort(where, text string) (int, error) {
	port, err := ParsePort(text)
	if err != nil {
		return 0, fmt.Errorf("%s: read port: %w", where, err)
	}
	return port, nil
}

// CheckSetting returns "port N", or "error: " and the message. The message
// of an error from fmt.Errorf already has the message of the wrapped error.
func CheckSetting(where, text string) string {
	port, err := ReadPort(where, text)
	if err != nil {
		return "error: " + err.Error()
	}
	return fmt.Sprintf("port %d", port)
}
