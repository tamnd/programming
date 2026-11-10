// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/read-an-environment-variable/.

package getenv

import (
	"os"
	"testing"
)

func TestCases(t *testing.T) {
	cases := []struct {
		name     string
		env      map[string]string
		key, def string
		want     string
	}{
		{"present", map[string]string{"APP_PORT": "8080"}, "APP_PORT", "80", "8080"},
		{"path-value", map[string]string{"APP_HOME": "/srv/app"}, "APP_HOME", "/opt/app", "/srv/app"},
		{"missing", nil, "APP_PORT", "80", "80"},
		{"empty-value", map[string]string{"APP_MODE": ""}, "APP_MODE", "production", ""},
		{"empty-default", nil, "APP_MODE", "", ""},
		{"spaces", map[string]string{"APP_TITLE": "  two  spaces  "}, "APP_TITLE", "none", "  two  spaces  "},
		{"equals", map[string]string{"APP_QUERY": "key=value=more"}, "APP_QUERY", "none", "key=value=more"},
		{"newline", map[string]string{"APP_TEXT": "first\nsecond"}, "APP_TEXT", "none", "first\nsecond"},
		{"non-ascii", map[string]string{"APP_GREETING": "xin chào, 日本語, 🙂"}, "APP_GREETING", "none", "xin chào, 日本語, 🙂"},
		{"case", map[string]string{"APP_PORT": "8080"}, "app_port", "80", "80"},
		{"inherited-name", nil, "toString", "none", "none"},
	}
	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			// t.Setenv restores the old value at the end of the test.
			t.Setenv(c.key, "")
			if err := os.Unsetenv(c.key); err != nil {
				t.Fatal(err)
			}
			for k, v := range c.env {
				t.Setenv(k, v)
			}
			if got := GetEnv(c.key, c.def); got != c.want {
				t.Errorf("GetEnv(%q, %q) = %q, want %q", c.key, c.def, got, c.want)
			}
		})
	}
}

// A Go string holds bytes, so a value that is not UTF-8 comes back unchanged.
func TestValueNotUTF8(t *testing.T) {
	t.Setenv("APP_NAME", "caf\xe9")
	if got := GetEnv("APP_NAME", "none"); got != "caf\xe9" {
		t.Errorf("GetEnv = %q", got)
	}
}
