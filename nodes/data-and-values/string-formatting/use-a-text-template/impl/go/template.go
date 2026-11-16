// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-text-template/.

// Package template fills a text template with $name, ${name} and $$, as in
// Python string.Template.
package template

import (
	"fmt"
	"regexp"
)

var placeholder = regexp.MustCompile(`\$(?:(\$)|\{([A-Za-z_][A-Za-z0-9_]*)\}|([A-Za-z_][A-Za-z0-9_]*))?`)

// Substitute returns template with $$ changed to $, and $name and ${name}
// changed to the value of name. The text that the function returns is used
// as it is, so a value is not read as a template. An unknown name or a $
// without a name is an error.
func Substitute(template string, names, values []string) (string, error) {
	table := make(map[string]string, len(names))
	for i, n := range names {
		table[n] = values[i]
	}
	var err error
	out := placeholder.ReplaceAllStringFunc(template, func(m string) string {
		g := placeholder.FindStringSubmatch(m)
		name := g[2] + g[3]
		switch {
		case g[1] != "":
			return "$"
		case name == "":
			if err == nil {
				err = fmt.Errorf("a $ without a name")
			}
			return m
		}
		v, ok := table[name]
		if !ok && err == nil {
			err = fmt.Errorf("unknown name: %s", name)
		}
		return v
	})
	if err != nil {
		return "", err
	}
	return out, nil
}
