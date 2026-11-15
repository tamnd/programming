// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-patch/.

// Package applypatch applies a unified diff to a text, with no fuzz and no
// offset.
package applypatch

import (
	"errors"
	"fmt"
	"os"
	"regexp"
	"strconv"
	"strings"
)

// ErrMalformed and ErrMismatch are the kinds of a PatchError.
var (
	ErrMalformed = errors.New("malformed")
	ErrMismatch  = errors.New("mismatch")
)

// PatchError is a malformed patch, or a hunk that does not match the text.
// Kind is ErrMalformed or ErrMismatch, and Line is the number of the line of
// the patch where the error is.
type PatchError struct {
	Kind error
	Line int
}

func (e *PatchError) Error() string {
	return fmt.Sprintf("%v at line %d of the patch", e.Kind, e.Line)
}

// Unwrap lets errors.Is find the kind.
func (e *PatchError) Unwrap() error { return e.Kind }

var hunkRe = regexp.MustCompile(`^@@ -(\d+)(?:,(\d+))? \+(\d+)(?:,(\d+))? @@`)

// SplitLines splits text after each LF. Each line keeps its LF, and a last
// line with no LF is a line too.
func SplitLines(text string) []string {
	var lines []string
	for text != "" {
		end := strings.IndexByte(text, '\n') + 1
		if end == 0 {
			end = len(text)
		}
		lines = append(lines, text[:end])
		text = text[end:]
	}
	return lines
}

type bodyLine struct {
	mark   byte
	text   string
	number int
	end    string
}

// count returns the number in s, or 1 if s is empty.
func count(s string) (int, error) {
	if s == "" {
		return 1, nil
	}
	return strconv.Atoi(s)
}

// ApplyPatch returns the text that the unified diff patch makes from the text
// oldText. Each hunk must match the lines of oldText at the start of its old
// range. The error is a *PatchError if the patch is malformed or a hunk does
// not match.
func ApplyPatch(oldText, patch string) (string, error) {
	lines := SplitLines(oldText)
	// The lines of the patch, with no LF. A CR stays in its line.
	p := SplitLines(patch)
	for i := range p {
		p[i] = strings.TrimSuffix(p[i], "\n")
	}
	var out strings.Builder
	cur := 0 // the index of the next line of oldText to use
	hunks := 0
	i := 0
	for i < len(p) {
		if !strings.HasPrefix(p[i], "@@") {
			i++ // a header line, or another line outside a hunk
			continue
		}
		m := hunkRe.FindStringSubmatch(p[i])
		if m == nil {
			return "", &PatchError{ErrMalformed, i + 1}
		}
		oldStart, err1 := strconv.Atoi(m[1])
		oldCount, err2 := count(m[2])
		newCount, err3 := count(m[4])
		if err1 != nil || err2 != nil || err3 != nil {
			return "", &PatchError{ErrMalformed, i + 1}
		}
		hunks++
		// An empty range starts after the line oldStart.
		pos := oldStart
		if oldCount > 0 {
			pos--
		}
		if pos < 0 {
			return "", &PatchError{ErrMalformed, i + 1}
		}
		if pos < cur || pos > len(lines) {
			return "", &PatchError{ErrMismatch, i + 1}
		}
		for ; cur < pos; cur++ {
			out.WriteString(lines[cur])
		}
		i++
		// The end of each line is an LF, or "" before a backslash line.
		var body []bodyLine
		oldLeft, newLeft := oldCount, newCount
		for oldLeft > 0 || newLeft > 0 {
			if i == len(p) {
				return "", &PatchError{ErrMalformed, i}
			}
			line := p[i]
			mark, text := byte(' '), ""
			if line != "" {
				mark, text = line[0], line[1:]
			}
			if mark == '\\' {
				if len(body) == 0 {
					return "", &PatchError{ErrMalformed, i + 1}
				}
				body[len(body)-1].end = "" // the line before has no LF
				i++
				continue
			}
			switch mark {
			case ' ':
				oldLeft--
				newLeft--
			case '-':
				oldLeft--
			case '+':
				newLeft--
			default:
				return "", &PatchError{ErrMalformed, i + 1}
			}
			if oldLeft < 0 || newLeft < 0 {
				return "", &PatchError{ErrMalformed, i + 1}
			}
			body = append(body, bodyLine{mark, text, i + 1, "\n"})
			i++
		}
		if i < len(p) && strings.HasPrefix(p[i], "\\") && len(body) > 0 {
			body[len(body)-1].end = ""
			i++
		}
		for _, b := range body {
			line := b.text + b.end
			if b.mark != '+' {
				if cur == len(lines) || lines[cur] != line {
					return "", &PatchError{ErrMismatch, b.number}
				}
				cur++
			}
			if b.mark != '-' {
				out.WriteString(line)
			}
		}
	}
	if hunks == 0 {
		return "", &PatchError{ErrMalformed, len(p)}
	}
	for ; cur < len(lines); cur++ {
		out.WriteString(lines[cur])
	}
	return out.String(), nil
}

// ApplyPatchFile returns ApplyPatch of the two UTF-8 files. If a file does
// not exist, the error matches fs.ErrNotExist.
func ApplyPatchFile(path, patchPath string) (string, error) {
	oldText, err := os.ReadFile(path)
	if err != nil {
		return "", err
	}
	patch, err := os.ReadFile(patchPath)
	if err != nil {
		return "", err
	}
	return ApplyPatch(string(oldText), string(patch))
}
