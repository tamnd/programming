// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/embed-a-file-in-a-program/.

// Package embedfile puts lines.txt into the program when it is built.
package embedfile

import (
	_ "embed"
	"strings"
)

// lines holds the bytes of lines.txt. The go command reads the file at
// build time; the program does not open it at run time.
//
//go:embed lines.txt
var lines string

// EmbeddedLine returns line n of lines.txt (1 is the line after the license
// header), with no line end. The bool is false when there is no such line.
func EmbeddedLine(n int) (string, bool) {
	all := strings.Split(lines, "\n")
	// all[0] is the license header, and the text after the last "\n" is empty.
	all = all[1 : len(all)-1]
	if n < 1 || n > len(all) {
		return "", false
	}
	return all[n-1], true
}
