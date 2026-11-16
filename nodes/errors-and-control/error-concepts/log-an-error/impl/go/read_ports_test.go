// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/log-an-error/.

package ports

import (
	"slices"
	"strings"
	"testing"
)

var cases = []struct {
	name  string
	texts []string
	ports []int
	log   string
}{
	{"one-bad", []string{"8080", "x", "443"}, []int{8080, 443}, "level=ERROR msg=\"read port failed\" line=2 err=\"not a number: 'x'\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"},
	{"all-good", []string{"80", "443"}, []int{80, 443}, "level=INFO msg=\"ports read\" ok=2 total=2\n"},
	{"out-of-range", []string{"70000"}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"out of range: 70000\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
	{"empty-list", []string{}, []int{}, "level=INFO msg=\"ports read\" ok=0 total=0\n"},
	{"empty-text", []string{""}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: ''\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
	{"boundary", []string{"1", "65535", "0"}, []int{1, 65535}, "level=ERROR msg=\"read port failed\" line=3 err=\"out of range: 0\"\nlevel=INFO msg=\"ports read\" ok=2 total=3\n"},
	{"newline", []string{"80\nlevel=INFO msg=\"all good\""}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\nlevel=INFO msg=\\\"all good\\\"'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
	{"backslash", []string{"C:\\port"}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: 'C:\\\\port'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
	{"carriage-return", []string{"80\r"}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '80\\r'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
	{"tab", []string{"8\t0"}, []int{}, "level=ERROR msg=\"read port failed\" line=1 err=\"not a number: '8\\t0'\"\nlevel=INFO msg=\"ports read\" ok=0 total=1\n"},
}

func TestReadPorts(t *testing.T) {
	for _, c := range cases {
		var log strings.Builder
		ports := ReadPorts(c.texts, NewLogger(&log))
		if !slices.Equal(ports, c.ports) {
			t.Errorf("%s: ports %v, want %v", c.name, ports, c.ports)
		}
		if log.String() != c.log {
			t.Errorf("%s: log %q, want %q", c.name, log.String(), c.log)
		}
	}
}
