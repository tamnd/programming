// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/parse-command-line-arguments/.

// Package parseargs parses the arguments of the tool
// "count [-v] [-n N] [--] [FILE...]".
package parseargs

import (
	"flag"
	"fmt"
	"io"
	"strconv"
	"strings"
)

// Args holds the result of ParseArgs.
type Args struct {
	Verbose bool
	Limit   int
	Files   []string
}

// ParseArgs parses args, the arguments without the program name. It returns
// an error for an unknown option, a missing value or a value that is not a
// decimal number. It does not print and does not exit.
func ParseArgs(args []string) (Args, error) {
	var a Args
	fs := flag.NewFlagSet("count", flag.ContinueOnError)
	fs.SetOutput(io.Discard)
	fs.BoolVar(&a.Verbose, "v", false, "print more")
	fs.BoolVar(&a.Verbose, "verbose", false, "print more")
	// flag.IntVar would accept 0x10, 010 and -5, so the value is a string
	// that the code converts.
	limit := "10"
	fs.StringVar(&limit, "n", limit, "the limit")
	fs.StringVar(&limit, "limit", limit, "the limit")
	if err := fs.Parse(args); err != nil {
		return Args{}, err
	}
	n, err := decimal(limit)
	if err != nil {
		return Args{}, err
	}
	a.Limit = n
	a.Files = fs.Args()
	return a, nil
}

// decimal converts a string of ASCII digits. strconv.Atoi alone would also
// accept a sign.
func decimal(s string) (int, error) {
	n, err := strconv.Atoi(s)
	if err != nil || strings.IndexFunc(s, func(r rune) bool { return r < '0' || r > '9' }) >= 0 {
		return 0, fmt.Errorf("invalid value %q for flag -limit: not a decimal number", s)
	}
	return n, nil
}
