// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-a-specific-line-from-a-file/.

// Package specificline reads one line of a text file by its number.
package specificline

import (
	"bufio"
	"errors"
	"io"
	"os"
	"strings"
)

// ReadLine returns line number of the UTF-8 file at path, and true. If there
// is no such line, it returns "" and false. Line 1 is the first line. A line
// ends at LF, and the LF is not part of it. A last line with no LF is a line
// too. The function reads the file only up to the line. A missing file gives
// an error that matches fs.ErrNotExist.
func ReadLine(path string, number int) (string, bool, error) {
	f, err := os.Open(path)
	if err != nil {
		return "", false, err
	}
	defer f.Close()
	if number < 1 {
		return "", false, nil
	}
	// bufio.Reader, not bufio.Scanner: ScanLines removes a CR before the LF,
	// and a Scanner has a limit on the length of a line.
	r := bufio.NewReader(f)
	for n := 1; ; n++ {
		// ReadString returns the line with its LF, or the last line with no
		// LF together with io.EOF.
		line, err := r.ReadString('\n')
		if err != nil && !errors.Is(err, io.EOF) {
			return "", false, err
		}
		if line == "" {
			return "", false, nil
		}
		if n == number {
			return strings.TrimSuffix(line, "\n"), true, nil
		}
		if err != nil {
			return "", false, nil
		}
	}
}
