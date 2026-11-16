// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

// Package port reads a TCP port number and reports a bad one with its own error type.
package port

import (
	"errors"
	"fmt"
)

// PortError reports that a text is not a valid TCP port number.
type PortError struct {
	Text   string
	Reason string
}

// Error makes *PortError satisfy the error interface.
func (e *PortError) Error() string {
	return fmt.Sprintf("invalid port '%s': %s", e.Text, e.Reason)
}

// ParsePort returns the port number in text. The type of the result is
// error, not *PortError, so a nil result is a nil error.
func ParsePort(text string) (int, error) {
	if text == "" {
		return 0, &PortError{Text: text, Reason: "empty"}
	}
	for _, c := range text {
		if c < '0' || c > '9' {
			return 0, &PortError{Text: text, Reason: "not a number"}
		}
	}
	port := 0
	for _, c := range text {
		port = port*10 + int(c-'0')
		if port > 65535 {
			return 0, &PortError{Text: text, Reason: "out of range"}
		}
	}
	if port == 0 {
		return 0, &PortError{Text: text, Reason: "out of range"}
	}
	return port, nil
}

// CheckPort returns "port N", or the message of the PortError.
func CheckPort(text string) string {
	port, err := ParsePort(text)
	var pe *PortError
	if errors.As(err, &pe) {
		return pe.Error()
	}
	if err != nil {
		panic(err) // ParsePort returns no other error.
	}
	return fmt.Sprintf("port %d", port)
}
