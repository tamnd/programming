// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

// Package ports reads port settings and logs the bad ones with log/slog.
package ports

import (
	"errors"
	"fmt"
	"io"
	"log/slog"
)

// The errors of ParsePort wrap one of these two values.
var (
	ErrNotANumber = errors.New("not a number")
	ErrOutOfRange = errors.New("out of range")
)

// ParsePort returns the port number in text.
func ParsePort(text string) (int, error) {
	if text == "" {
		return 0, fmt.Errorf("%w: '%s'", ErrNotANumber, text)
	}
	for _, c := range text {
		if c < '0' || c > '9' {
			return 0, fmt.Errorf("%w: '%s'", ErrNotANumber, text)
		}
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

// NewLogger returns a logger that writes logfmt lines to w, without the
// time, so that the output is the same on each run.
func NewLogger(w io.Writer) *slog.Logger {
	removeTime := func(groups []string, a slog.Attr) slog.Attr {
		if a.Key == slog.TimeKey && len(groups) == 0 {
			return slog.Attr{}
		}
		return a
	}
	return slog.New(slog.NewTextHandler(w, &slog.HandlerOptions{ReplaceAttr: removeTime}))
}

// ReadPorts returns the good ports. It logs each bad text at the error
// level, then a summary at the info level. The text handler puts a value
// in quotes and escapes it when it has a space or a control character.
func ReadPorts(texts []string, log *slog.Logger) []int {
	ports := make([]int, 0, len(texts))
	for i, text := range texts {
		port, err := ParsePort(text)
		if err != nil {
			// The error is handled here: it is logged once and not returned.
			log.Error("read port failed", "line", i+1, "err", err)
			continue
		}
		ports = append(ports, port)
	}
	log.Info("ports read", "ok", len(ports), "total", len(texts))
	return ports
}
