// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/diff-two-texts/.

// Package difftexts compares two texts line by line.
package difftexts

import (
	"os"
	"strings"
)

// SplitLines splits text at each LF. A final LF does not start a new line.
func SplitLines(text string) []string {
	// strings.Split("a\n", "\n") is ["a" ""], and strings.Split("", "\n") is [""].
	lines := strings.Split(text, "\n")
	if lines[len(lines)-1] == "" {
		lines = lines[:len(lines)-1]
	}
	return lines
}

// DiffLines returns the edit script that changes the lines of oldText into
// the lines of newText. Each line of the result is a space and a line that
// stays, a - and a line of oldText that goes, or a + and a line of newText
// that comes, with an LF at the end. The script has the smallest number of -
// and + lines. When two scripts are equally short, a - comes before a +.
func DiffLines(oldText, newText string) string {
	a, b := SplitLines(oldText), SplitLines(newText)
	n, m := len(a), len(b)
	w := m + 1
	// lcs[i*w+j] is the length of a longest common subsequence of a[i:] and
	// b[j:]. Row n and column m stay 0.
	lcs := make([]int32, (n+1)*w)
	for i := n - 1; i >= 0; i-- {
		for j := m - 1; j >= 0; j-- {
			if a[i] == b[j] {
				lcs[i*w+j] = lcs[(i+1)*w+j+1] + 1
			} else {
				lcs[i*w+j] = max(lcs[(i+1)*w+j], lcs[i*w+j+1])
			}
		}
	}
	var out strings.Builder
	line := func(mark byte, s string) {
		out.WriteByte(mark)
		out.WriteString(s)
		out.WriteByte('\n')
	}
	i, j := 0, 0
	for i < n && j < m {
		switch {
		case a[i] == b[j]:
			line(' ', a[i])
			i++
			j++
		case lcs[(i+1)*w+j] >= lcs[i*w+j+1]:
			line('-', a[i])
			i++
		default:
			line('+', b[j])
			j++
		}
	}
	for ; i < n; i++ {
		line('-', a[i])
	}
	for ; j < m; j++ {
		line('+', b[j])
	}
	return out.String()
}

// DiffFiles returns DiffLines of the two files. A missing file gives an error
// that matches fs.ErrNotExist.
func DiffFiles(oldPath, newPath string) (string, error) {
	oldData, err := os.ReadFile(oldPath)
	if err != nil {
		return "", err
	}
	newData, err := os.ReadFile(newPath)
	if err != nil {
		return "", err
	}
	return DiffLines(string(oldData), string(newData)), nil
}
