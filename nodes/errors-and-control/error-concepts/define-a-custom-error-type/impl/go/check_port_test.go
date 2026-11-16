// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-custom-error-type/.

package port

import (
	"errors"
	"testing"
)

var cases = []struct {
	name, text, want string
}{
	{"valid", "8080", "port 8080"},
	{"not-a-number", "x80", "invalid port 'x80': not a number"},
	{"too-large", "65536", "invalid port '65536': out of range"},
	{"empty", "", "invalid port '': empty"},
	{"zero", "0", "invalid port '0': out of range"},
	{"one", "1", "port 1"},
	{"largest", "65535", "port 65535"},
	{"leading-zeros", "0080", "port 80"},
	{"negative", "-1", "invalid port '-1': not a number"},
	{"leading-space", " 80", "invalid port ' 80': not a number"},
	{"overflow", "99999999999999999999", "invalid port '99999999999999999999': out of range"},
}

func TestCheckPort(t *testing.T) {
	for _, c := range cases {
		if got := CheckPort(c.text); got != c.want {
			t.Errorf("%s: got %q, want %q", c.name, got, c.want)
		}
	}
}

func TestFields(t *testing.T) {
	_, err := ParsePort("x80")
	var pe *PortError
	if !errors.As(err, &pe) || pe.Text != "x80" || pe.Reason != "not a number" {
		t.Errorf("got %#v", err)
	}
	if _, err := ParsePort("80"); err != nil {
		t.Errorf("got %#v, want nil", err)
	}
}
