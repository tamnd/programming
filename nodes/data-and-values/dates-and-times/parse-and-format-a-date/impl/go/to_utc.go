// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-and-format-a-date/.

package toutc

import (
	"fmt"
	"regexp"
	"strings"
	"time"
)

// rfc3339 is the date-time of RFC 3339 section 5.6, with a fraction of 1 to 3
// digits. time.Parse with time.RFC3339 accepts more than this (a comma before
// the fraction, the offsets +24:00 and +07:60), so the pattern checks the
// syntax first.
var rfc3339 = regexp.MustCompile(`^[0-9]{4}-[0-9]{2}-[0-9]{2}[Tt]([01][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9](\.[0-9]{1,3})?([Zz]|[+-]([01][0-9]|2[0-3]):[0-5][0-9])$`)

// ToUTC returns the instant of an RFC 3339 timestamp in UTC, as
// YYYY-MM-DDTHH:MM:SS.sssZ. It returns an error when timestamp is not an RFC
// 3339 date-time with an offset, or when its date is not in the calendar.
func ToUTC(timestamp string) (string, error) {
	if !rfc3339.MatchString(timestamp) {
		return "", fmt.Errorf("not an RFC 3339 date-time: %q", timestamp)
	}
	// time.Parse rejects 2026-02-29 with "day out of range".
	t, err := time.Parse(time.RFC3339, strings.ToUpper(timestamp))
	if err != nil {
		return "", err
	}
	// A layout is the reference time Mon Jan 2 15:04:05 MST 2006, written in
	// the form to print. "Z07:00" prints Z for UTC.
	return t.UTC().Format("2006-01-02T15:04:05.000Z07:00"), nil
}
