// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-an-error-with-context/.

package setting

import (
	"errors"
	"testing"
)

var cases = []struct {
	name, where, text, want string
}{
	{"valid", "app.conf:3", "8080", "port 8080"},
	{"not-a-number", "app.conf:3", "x", "error: app.conf:3: read port: not a number: 'x'"},
	{"too-large", "app.conf:7", "70000", "error: app.conf:7: read port: out of range: 70000"},
	{"empty", "app.conf:1", "", "error: app.conf:1: read port: not a number: ''"},
	{"zero", "app.conf:2", "0", "error: app.conf:2: read port: out of range: 0"},
	{"largest", "app.conf:2", "65535", "port 65535"},
	{"leading-space", "app.conf:4", " 80", "error: app.conf:4: read port: not a number: ' 80'"},
	{"overflow", "app.conf:5", "99999999999999999999", "error: app.conf:5: read port: out of range: 99999999999999999999"},
	{"negative", "app.conf:6", "-1", "error: app.conf:6: read port: not a number: '-1'"},
}

func TestCheckSetting(t *testing.T) {
	for _, c := range cases {
		if got := CheckSetting(c.where, c.text); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestCause(t *testing.T) {
	_, err := ReadPort("app.conf:3", "x")
	if !errors.Is(err, ErrNotANumber) {
		t.Errorf("errors.Is(%v, ErrNotANumber) is false", err)
	}
}
