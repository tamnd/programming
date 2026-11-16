// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-an-enumeration/.

// Package cycle defines an enumeration with iota.
package cycle

// Day is a day of the week. Go has no enum type; a named integer type and
// a block of constants take its place.
type Day int

const (
	Monday Day = iota
	Tuesday
	Wednesday
	Thursday
	Friday
	Saturday
	Sunday
	numDays // the number of days; not a day
)

var names = [numDays]string{"monday", "tuesday", "wednesday", "thursday", "friday", "saturday", "sunday"}

// String gives the name of the day. fmt calls it when it prints a Day.
func (d Day) String() string {
	if d < 0 || d >= numDays {
		return "Day(?)"
	}
	return names[d]
}

// Parse gives the day with the name s, or false.
func Parse(s string) (Day, bool) {
	for d := Monday; d < numDays; d++ {
		if names[d] == s {
			return d, true
		}
	}
	return 0, false
}

// Cycle returns steps day names from start on, with monday after sunday.
func Cycle(start string, steps int) []string {
	d, ok := Parse(start)
	if !ok {
		return []string{}
	}
	out := make([]string, 0, steps)
	for k := range steps {
		out = append(out, ((d + Day(k)) % numDays).String())
	}
	return out
}
