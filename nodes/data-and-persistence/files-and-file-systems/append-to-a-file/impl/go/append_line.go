// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-file/.

// Package appendline adds lines to the end of a file.
package appendline

import (
	"errors"
	"os"
	"strings"
)

// ErrInvalidLine reports a line that holds "\n" or "\r".
var ErrInvalidLine = errors.New("the line must not hold \\n or \\r")

// AppendLine adds line and one "\n" to the end of the file, in one write call.
func AppendLine(path, line string) error {
	if strings.ContainsAny(line, "\r\n") {
		return ErrInvalidLine
	}
	// O_APPEND moves the offset to the end of the file before each write.
	f, err := os.OpenFile(path, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0o666)
	if err != nil {
		return err
	}
	_, werr := f.Write([]byte(line + "\n"))
	cerr := f.Close()
	if werr != nil {
		return werr
	}
	return cerr
}
