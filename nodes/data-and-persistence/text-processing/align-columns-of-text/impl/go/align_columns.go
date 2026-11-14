// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/align-columns-of-text/.

// Package aligncolumns aligns the columns of a text in which a $ separates
// the fields.
package aligncolumns

import (
	"errors"
	"strings"
	"unicode/utf8"
)

// ErrBadAlign is the error when align is not left, right or center.
var ErrBadAlign = errors.New("align is not left, right or center")

// AlignColumns returns text with its fields in columns. A line ends at LF.
// Each $ in a line separates two fields, and a $ at the end of a line does not
// start a new field. An empty line has no fields. The width of a column is the
// largest number of code points of a field in it. Each field is padded with
// spaces to the width of its column, on the right (left), on the left
// (right), or on the two sides (center), with the odd space on the right. One
// space separates two columns. The padding on the right of the last field of
// a line is not written. Each line of the result ends with LF.
func AlignColumns(text, align string) (string, error) {
	if align != "left" && align != "right" && align != "center" {
		return "", ErrBadAlign
	}
	lines := strings.Split(text, "\n")
	if lines[len(lines)-1] == "" {
		// A final LF ends the last line. It does not start a new line.
		lines = lines[:len(lines)-1]
	}
	rows := make([][]string, len(lines))
	var widths []int
	for i, line := range lines {
		rows[i] = splitFields(line)
		for j, field := range rows[i] {
			if j == len(widths) {
				widths = append(widths, 0)
			}
			// len counts bytes. RuneCountInString counts code points.
			widths[j] = max(widths[j], utf8.RuneCountInString(field))
		}
	}
	var b strings.Builder
	for _, row := range rows {
		for j, field := range row {
			pad := widths[j] - utf8.RuneCountInString(field)
			left := 0
			switch align {
			case "right":
				left = pad
			case "center":
				left = pad / 2
			}
			right := pad - left
			if j == len(row)-1 {
				right = 0
			}
			if j > 0 {
				b.WriteByte(' ')
			}
			b.WriteString(strings.Repeat(" ", left))
			b.WriteString(field)
			b.WriteString(strings.Repeat(" ", right))
		}
		b.WriteByte('\n')
	}
	return b.String(), nil
}

// splitFields returns the fields of a line: none for an empty line.
func splitFields(line string) []string {
	if line == "" {
		return nil
	}
	fields := strings.Split(line, "$")
	if strings.HasSuffix(line, "$") {
		fields = fields[:len(fields)-1]
	}
	return fields
}
