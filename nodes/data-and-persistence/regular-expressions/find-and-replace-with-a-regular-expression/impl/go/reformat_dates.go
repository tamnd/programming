// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-and-replace-with-a-regular-expression/.

// Package reformatdates finds dates with a regular expression and writes them in another order.
package reformatdates

import (
	"regexp"
	"strings"
)

// datePattern returns the pattern of a date YYYY<sep>MM<sep>DD. RE2 has no
// lookaround, so the pattern does not check the digits next to the date.
func datePattern(sep string) *regexp.Regexp {
	// QuoteMeta makes each special character of sep match only itself.
	s := regexp.QuoteMeta(sep)
	return regexp.MustCompile(`(?P<year>[0-9]{4})` + s + `(?P<month>[0-9]{2})` + s + `(?P<day>[0-9]{2})`)
}

func isDigit(text string, i int) bool {
	return i >= 0 && i < len(text) && '0' <= text[i] && text[i] <= '9'
}

// eachDate calls f with the submatch indexes of each date in text that has no
// digit next to it. The indexes are offsets in text.
func eachDate(re *regexp.Regexp, text string, f func(m []int)) {
	for pos := 0; pos < len(text); {
		m := re.FindStringSubmatchIndex(text[pos:])
		if m == nil {
			return
		}
		for i := range m {
			m[i] += pos
		}
		if isDigit(text, m[0]-1) || isDigit(text, m[1]) {
			// A date can start inside this match, so search again one byte later.
			// m[0] is an ASCII digit, so m[0]+1 starts a character.
			pos = m[0] + 1
			continue
		}
		f(m)
		pos = m[1]
	}
}

// ReformatDates changes each date YYYY<fromSep>MM<fromSep>DD in text to
// DD<toSep>MM<toSep>YYYY.
func ReformatDates(text, fromSep, toSep string) string {
	re := datePattern(fromSep)
	// In a template, $ starts a reference such as $1 or ${name}, and $$ is one $.
	sep := strings.ReplaceAll(toSep, "$", "$$")
	template := "${day}" + sep + "${month}" + sep + "${year}"
	var out []byte
	last := 0
	eachDate(re, text, func(m []int) {
		out = append(out, text[last:m[0]]...)
		out = re.ExpandString(out, template, text, m)
		last = m[1]
	})
	return string(append(out, text[last:]...))
}

// FindDates returns [year, month, day] for each date YYYY<sep>MM<sep>DD in text.
func FindDates(text, sep string) [][]string {
	dates := [][]string{}
	eachDate(datePattern(sep), text, func(m []int) {
		dates = append(dates, []string{text[m[2]:m[3]], text[m[4]:m[5]], text[m[6]:m[7]]})
	})
	return dates
}
